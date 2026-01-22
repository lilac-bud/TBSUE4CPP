# Turn Based Strategy (C++ Version)
Данный проект является развитием [работы](https://github.com/lilac-bud/TBSUE4), проделанной в рамках написания магистерской диссертации. Модуль, написанный на языке Python с использованием tensorflow, был переписан в виде библиотеки на языке C++, что значительно упрощает установку игры на других компьютерах. Сама игра, для которой ранее был использован Blueprints, также была большей частью переписана на C++.

Скомпилированная версия игры скоро будет добавлена в виде релиза. Обучение компьютерного игрока (нейронной сети) можно запускать **только** в этой версии. Если обучение запустить в редакторе, это может привести к нехватке памяти и вылету ОС. 

## Библиотека Q
Написана при помощи библиотек [xtensor](https://github.com/xtensor-stack/xtensor), [xtl](https://github.com/xtensor-stack/xtl) (необходима для работы с xtensor) и [nlohmann/json](https://github.com/nlohmann/json) (для сохранения весов и других параметров) на C++20. Является реализацией [алгоритма DQN](https://github.com/lilac-bud/TBSUE4?tab=readme-ov-file#learning). Для ускорения вычислений используются потоки. Библиотека имеет один хедер Q.h, код которого приведён ниже.
```C++
#pragma once
#include <vector>
#include <string>

class Q final
{
private:
	class QPrivate* QP;

public:
	Q(std::size_t field_height, std::size_t field_width, std::size_t channels_number, 
		const std::string player_id, const std::string filepath);
	~Q();
	void soft_reset();
	int call_network(float prev_reward, const std::vector<float>& state, 
		const std::vector<float>& actions, std::size_t actions_number);
	int call_network_debug(float prev_reward, std::size_t actions_number);
	int call_network_debug(float prev_reward);
};
```
Для вызова конструктора класса Q необходимо указать размеры игрового поля и число каналов, которое соответствует количеству типов информации о ячейке игрового поля (например, находится ли в ней вражеский юнит). Также нужно указать ID игрока, для которого Q будет выбирать действие, и путь к папке для сохранений.

Метод soft_reset вызывается по окончании игры (эпизода обучения) для того, чтобы модель могла быть использована для следующей игры.

Метод call_network принимает на вход предыдущую награду, вектор, являющийся представлением текущего состояния (должен быть размера field_height * field_width * channels_number), ещё один вектор, соответствующий возможным действиям и число действий (то есть размер вектора actions должен быть field_height * field_width * channels_number * actions_number). Возвращает индекс выбранного действия.

Методы call_network_debug используются для отладки и генерируют инпут в зависимости от параметров.

Загрузка и сохранение не вызываются напрямую.

## QNetworkPlugin
Плагин представляет собой посредника между игрой, разработанной на Unreal Engine, и библиотекой Q и включает в себя структуру NetworkInput и компонент QNetworkComponent.
```C++
#pragma once

#include "CoreMinimal.h"
#include "NetworkInput.generated.h"

USTRUCT()
struct FNetworkInput
{
	GENERATED_BODY()

	float Reward;
	UPROPERTY()
	TArray<float> State;
	UPROPERTY()
	TArray<float> Actions;
	int32 ActionsNumber;

	FNetworkInput() : Reward(0), ActionsNumber(0) {};
};
```
NetworkInput включает в себя все необходимые параметры для вызова метода call_network и используется для передачи данных в компонент.
```C++
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QNetworkComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QNETWORKPLUGIN_API UQNetworkComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	class Q* Network;

protected:
	virtual void BeginPlay() override;

public:	
	UQNetworkComponent();
	
	void Initialize(int32 FieldHeight, int32 FieldWidth, int32 Channels, int32 PlayerID, FString Filepath);
	void SoftReset();
	int32 CallQNetwork(const struct FNetworkInput& NetworkInput);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
```
Компонент содержит указатель на объект Q, который в свою очередь создаётся при вызове метода Initialize. SoftReset и CallQNetwork соответствуют методам soft_reset и call_network класса Q. CallQNetwork также служит для приведения типов данных, специфичных для Unreal Engine, к стандартным для C++.

При вызове EndPlay объект Network удаляется.

## AIPawn
Как и в [предыдущей версии](https://github.com/lilac-bud/TBSUE4?tab=readme-ov-file#aipawn) представляет собой компьютерного игрока. Содержит описанный выше компонент QNetworkComponent. Ход компьютерного игрока выглядит следующим образом:
```C++
void AAIPawn::Turn()
{
	Super::Turn();
	UpdateCurrentState();
	for (AUnit* SelectedUnit : UnitsComponent->Units)
	{
		if (SelectedUnit->ReachableTiles.Num() <= 1) continue;
		GenerateActions(SelectedUnit);
		int32 ActionIndex = QNetworkComponent->CallQNetwork(BuildNetworkInput());
		CurrentState = PossibleActions[ActionIndex];
		PossibleActions.Empty();
		ATile* TargetTile = SelectedUnit->ReachableTiles[ActionIndex];
		if (TargetTile != SelectedUnit->GetTile())
			Reward = SelectedUnit->Act(TargetTile) ? 0.5f : 0.0f;
		if (!bCanPlay) return;
	}
	FTimerHandle UnusedHandle;
	check(IsValid(GameMode));
	GetWorldTimerManager().SetTimer(UnusedHandle, GameMode, &ATBSGameMode::CycleTurns, 0.01f, false);
}
```
В начале хода обновляется текущее состояние игрового поля, затем для каждого юнита, способного двигаться на этом ходу:
* Генерируются все возможные действия, каждое из которых является потенциальным состоянием для той или иной целевой клетки, которая может быть выбрана для перемещения или атаки.
* На основе текущего значения Reward, текущего состояния и сгенерированных действий формируется NetworkInput, на основании которого QNetworkComponent возвращает индекс выбранного действия.
* Выбранное действие устанавливается как текущее состояние. Все остальные действия удаляются.
* Выбранному действию соответствует целевая ячейка, находящаяся среди достижимых юнитом клеток по тому же индексу.
* Если юнит не располагается на целевой клетке, то он либо перемещается на неё, либо атакует вражеского юнита, что находится на ней. В случае успешного уничтожения врага награда устанавливается как 0,5.
* В том случае, если компьютерный игрок побеждает (а игрок может победить только на своём ходу), он больше не может продолжать игру, кроме того, нет необходимости ходить остальными юнитами.

После того, как все юниты совершили действие, ход переходит к следующему игроку.

В случае победы компьютерного игрока вызывается следующий метод:
```C++
void AAIPawn::Win()
{
	bCanPlay = false;
	UpdateCurrentState();
	Reward = 1.0f;
	QNetworkComponent->CallQNetwork(BuildNetworkInput());
	FTimerHandle UnusedHandle;
	check(IsValid(GameMode));
	GetWorldTimerManager().SetTimer(UnusedHandle, GameMode, &ATBSGameMode::EndGame, 0.5f, false);
}
```
В этом случае указывается, что игрок больше не может играть, затем обновляется текущее состояние игрового поля, а награда указывается как 1. После чего формируется NetworkInput, который отправляется в QNetworkComponent. После чего в GameMode вызывается EndGame.

В случае проигрыша вызывается метод LoseCondition, аналогичный методу Win. Только в этом случае награда устанавливается как 0, и вместо EndGame вызывается PlayerLost.

## Использование приложения
По большей части не отличается от [предыдущей версии](https://github.com/lilac-bud/TBSUE4?tab=readme-ov-file#use). Для удобства использования были добавлены индикаторы здоровья юнитов, а также вывод номера текущей итерации на экран (если общее количество итераций было установлено больше единицы).
<img width="1920" height="1080" alt="0_new_interface" src="https://github.com/user-attachments/assets/2e565000-5241-4e49-8d6d-961683b94d9f" />

## Перспективы развития
Для дальнейшего тестирования алгоритма можно усложнить саму игру. К примеру, можно добавить различные типы юнитов с дополнительными действиями и города, для которых нужно будет определять производство. Также можно улучшить игровое поле, добавив различные типы ячеек, препятствия и ресурсы (всё это, разумеется, увеличит количество каналов информации).

В отношении самой нейронной сети наиболее значительным улучшением была бы имплементация распределённой архитектуры, где обучение отделяется от самого игрового процесса. Также стоит заметить, что на данный момент библиотека Q – статическая, что крайне осложняет внесение изменений. В дальнейшем планируется использование динамической библиотеки.

Наконец, необходимо внести дополнительные изменения в интерфейс, такие как пауза и возможность выхода из программы в процессе игры.

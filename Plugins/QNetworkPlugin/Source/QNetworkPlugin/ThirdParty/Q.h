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
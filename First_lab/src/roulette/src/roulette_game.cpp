#include <ros/ros.h>
#include "std_msgs/String.h"
#include "roulette/PlayRoulette.h"

using namespace std;


ros::ServiceServer server;
ros::Publisher publisher;
ros::Subscriber subscriber;


bool check_result(roulette::PlayRoulette::Request &req, roulette::PlayRoulette::Response &res) {

	int dropped_number = rand()%37+0;
	string dropped_color;
	string winner = "win";
	string loser = "lose";
	int win_message_number = rand()%4+1;
	int lose_message_number = rand()%4+1;
	
	string first_message = "Congratulations!!!";
	string second_message = "Today you are the best!!!";
	string third_message = "Look at this lucky guy!!!";
	string fourth_message = "Don't be upset, next time you will definitely win!";
	string fifth_message = "Don't worry, what doaesn't kill us makes us stronger!";
	string sixth_message = "This must be a mistake! Just try again!";
	string error_message = "Something goes wrong. Maybe you put incorrect number? Try again!";
	
	if ((dropped_number == 1)||(dropped_number == 3)||(dropped_number == 5)||
	    (dropped_number == 7)||(dropped_number == 9)||(dropped_number == 12)||
	    (dropped_number == 14)||(dropped_number == 16)||(dropped_number == 18)||
	    (dropped_number == 19)||(dropped_number == 21)||(dropped_number == 23)||
	    (dropped_number == 25)||(dropped_number == 27)||(dropped_number == 30)||
	    (dropped_number == 32)||(dropped_number == 34)||(dropped_number == 36))
	{
		dropped_color = "red";
	}
	if ((dropped_number == 2)||(dropped_number == 4)||(dropped_number == 6)||
	    (dropped_number == 8)||(dropped_number == 10)||(dropped_number == 11)||
	    (dropped_number == 13)||(dropped_number == 15)||(dropped_number == 17)||
	    (dropped_number == 20)||(dropped_number == 22)||(dropped_number == 24)||
	    (dropped_number == 26)||(dropped_number == 28)||(dropped_number == 29)||
	    (dropped_number == 31)||(dropped_number == 33)||(dropped_number == 35))
	{
		dropped_color = "black";
	}	
	if (dropped_number == 0) 
	{
		dropped_color = "green";
	}
	
	if ((req.number > 36)||(req.number < 0)) 
	{
		res.result = loser;
		res.message = error_message;
	}
	if ((req.number == dropped_number)&&(req.color == dropped_color)) 
	{
		res.result = winner;
		if (win_message_number == 1) 
		{
			res.message = first_message;
		}
		if (win_message_number == 2) 
		{
			res.message = second_message;
		}
		if (win_message_number == 3) 
		{
			res.message = third_message;
		}
		
	}
	else 
	{
		res.result = loser;
		if (lose_message_number == 1) 
		{
			res.message = fourth_message;
		}
		if (lose_message_number == 2) 
		{
			res.message = fifth_message;
		}
		if (lose_message_number == 3) 
		{
			res.message = sixth_message;
		}
	}
	
	ROS_INFO("request: %s, %d", req.color, req.number);
    	ROS_INFO("sending back response:[%s]", res.result);
	
	
	ros::spinOnce();

    	return true;
	
}

void recieve(const roulette::PlayRoulette::Response &res) {

    	string winner = "win";
	string loser = "lose";
	
    	if (res.result == winner)
    	{
        	ROS_INFO("Win!");
        	ROS_INFO("%s", res.message);
        }
    	else
    	{
       	ROS_INFO("Lose!");
       	ROS_INFO("%s", res.message);
	}
    	return;
}


int main(int argc, char **argv) {

    ros::init(argc, argv, "roulette_situation");
    ros::NodeHandle n;

    server = n.advertiseService("play_roulette", check_result);
    publisher = n.advertise<std_msgs::String>("/name", 1000);
    subscriber = n.subscribe("/name", 1000, recieve);

    ROS_INFO("Enter the color and number");
    ros::spin();
    return 0;

}

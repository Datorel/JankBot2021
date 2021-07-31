#include "../include/main.h"
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}
//checks for button press to change mode
int changeMode(int mode);
//initializes motors and Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

//init drive motors
pros::Motor lFDrive(9, pros::E_MOTOR_GEARSET_18, false);
pros::Motor rFDrive(2, pros::E_MOTOR_GEARSET_18, true);
pros::Motor lRDrive(10, pros::E_MOTOR_GEARSET_18, false);
pros::Motor rRDrive(1, pros::E_MOTOR_GEARSET_18, true);

//init auxillary motors
pros::Motor lift(6, pros::E_MOTOR_GEARSET_36, false);
pros::Motor rIn(8);
pros::Motor lIn(7);

void config() {
	//configure motors
	lFDrive.set_current_limit(1500);
	rFDrive.set_current_limit(1500);
	lRDrive.set_current_limit(1500);
	rRDrive.set_current_limit(1500);
}


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	config();
	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	while (true) {
		int driveMode = 0;

		switch(driveMode){
			case 0:
				pros::lcd::set_text(1, "Tank Drive");

				lFDrive.move(master.get_analog(ANALOG_LEFT_Y));
				lRDrive.move(master.get_analog(ANALOG_LEFT_Y));
				rFDrive.move(master.get_analog(ANALOG_RIGHT_Y));
				rRDrive.move(master.get_analog(ANALOG_RIGHT_Y));

				if (master.get_digital(DIGITAL_R1)) {
					rIn.move_velocity(100);
					lIn.move_velocity(100);
				}
				else if (master.get_digital(DIGITAL_R2)) {
					rIn.move_velocity(-50);
					lIn.move_velocity(-50);
				}
				else {
					rIn.move_velocity(0);
					lIn.move_velocity(0);
				}

				if (master.get_digital(DIGITAL_L1)) {
					lift.move_velocity(100);
				}
				else if (master.get_digital(DIGITAL_L2)) {
					lift.move_velocity(-50);
				}
				else {
					lift.move_velocity(0);
				}


			break;
			case 1:
				pros::lcd::set_text(1, "Arcade Drive");
			break;
			case 2:
				pros::lcd::set_text(1, "Leftie Arcade");
			break;
			case 3:
				pros::lcd::set_text(1, "Single Stick Arcade");
				pros::lcd::set_text(2, "Press again to toggle side.");
			break;
			case 4:
				pros::lcd::set_text(1, "Single Stick Arcade");
				pros::lcd::set_text(2, "Press again to toggle side.");
			break;
			default:
			break;
		}

		pros::delay(20);
	}
}

int changeMode(int mode) {
	if(master.get_digital_new_press(DIGITAL_UP)) {
		mode = 0;
	}
	if(master.get_digital_new_press(DIGITAL_LEFT)) {
		mode = 2;
	}
	if(master.get_digital_new_press(DIGITAL_RIGHT)) {
		mode = 1;
	}
	if(master.get_digital_new_press(DIGITAL_DOWN)) {
		if(mode == 3) {
			mode = 4;
		}
		else if (mode == 4) {
			mode = 3;
		}
	}
}

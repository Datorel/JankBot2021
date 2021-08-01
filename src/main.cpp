#include "../include/main.h"

lv_obj_t * myButton;
lv_obj_t * myButtonLabel;
lv_obj_t * myLabel;

lv_style_t myButtonStyleREL; //relesed style
lv_style_t myButtonStylePR; //pressed style

static lv_res_t btn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn); //id usefull when there are multiple buttons

    if(id == 0)
    {
        char buffer[100];
		sprintf(buffer, "button was clicked %i milliseconds from start", pros::millis());
		lv_label_set_text(myLabel, buffer);
    }

    return LV_RES_OK;
}

//checks for button press to change mode
int changeMode(int mode);
//initializes motors and Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

//init drive motors
pros::Motor lFDrive(9, pros::E_MOTOR_GEARSET_18, true);
pros::Motor rFDrive(2, pros::E_MOTOR_GEARSET_18, false);
pros::Motor lRDrive(10, pros::E_MOTOR_GEARSET_18, true);
pros::Motor rRDrive(1, pros::E_MOTOR_GEARSET_18, false);

//init auxillary motors
pros::Motor lift(6, pros::E_MOTOR_GEARSET_36, true);
pros::Motor claw(7, pros::E_MOTOR_GEARSET_36, true);

void config() {
	//configure motors
	lFDrive.set_current_limit(1000);
	rFDrive.set_current_limit(1000);
	lRDrive.set_current_limit(1000);
	rRDrive.set_current_limit(1000);

	lift.set_current_limit(1500);
	claw.set_current_limit(1000);

	lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	config();

	lv_style_copy(&myButtonStyleREL, &lv_style_plain);
    myButtonStyleREL.body.main_color = LV_COLOR_MAKE(150, 0, 0);
    myButtonStyleREL.body.grad_color = LV_COLOR_MAKE(0, 0, 150);
    myButtonStyleREL.body.radius = 0;
    myButtonStyleREL.text.color = LV_COLOR_MAKE(255, 255, 255);

    lv_style_copy(&myButtonStylePR, &lv_style_plain);
    myButtonStylePR.body.main_color = LV_COLOR_MAKE(255, 0, 0);
    myButtonStylePR.body.grad_color = LV_COLOR_MAKE(0, 0, 255);
    myButtonStylePR.body.radius = 0;
    myButtonStylePR.text.color = LV_COLOR_MAKE(255, 255, 255);

    myButton = lv_btn_create(lv_scr_act(), NULL); //create button, lv_scr_act() is deafult screen object
    lv_obj_set_free_num(myButton, 0); //set button is to 0
    lv_btn_set_action(myButton, LV_BTN_ACTION_CLICK, btn_click_action); //set function to be called on button click
    lv_btn_set_style(myButton, LV_BTN_STYLE_REL, &myButtonStyleREL); //set the relesed style
    lv_btn_set_style(myButton, LV_BTN_STYLE_PR, &myButtonStylePR); //set the pressed style
    lv_obj_set_size(myButton, 200, 50); //set the button size
    lv_obj_align(myButton, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10); //set the position to top mid

    myButtonLabel = lv_label_create(myButton, NULL); //create label and puts it inside of the button
    lv_label_set_text(myButtonLabel, "Click the Button"); //sets label text

    myLabel = lv_label_create(lv_scr_act(), NULL); //create label and puts it on the screen
    lv_label_set_text(myLabel, "Button has not been clicked yet"); //sets label text
    lv_obj_align(myLabel, NULL, LV_ALIGN_CENTER, 10, 0); //set the position to center

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
	int clawWasOpen;
	int driveMode = 0;

	while (true) {

		switch(driveMode){
			case 0:
				pros::lcd::set_text(1, "Tank Drive");

				lFDrive.move(master.get_analog(ANALOG_LEFT_Y));
				lRDrive.move(master.get_analog(ANALOG_LEFT_Y));
				rFDrive.move(master.get_analog(ANALOG_RIGHT_Y));
				rRDrive.move(master.get_analog(ANALOG_RIGHT_Y));

			break;
			case 1:
				pros::lcd::set_text(1, "Arcade Drive");

				lFDrive.move(master.get_analog(ANALOG_RIGHT_Y) + master.get_analog(ANALOG_LEFT_X));
				lRDrive.move(master.get_analog(ANALOG_RIGHT_Y) + master.get_analog(ANALOG_LEFT_X));
				rFDrive.move(master.get_analog(ANALOG_RIGHT_Y) - master.get_analog(ANALOG_LEFT_X));
				rRDrive.move(master.get_analog(ANALOG_RIGHT_Y) - master.get_analog(ANALOG_LEFT_X));

			break;
			case 2:
				pros::lcd::set_text(1, "Leftie Arcade");

				lFDrive.move(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X));
				lRDrive.move(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X));
				rFDrive.move(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X));
				rRDrive.move(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X));
			break;
			case 3:
				pros::lcd::set_text(1, "Single Stick Arcade");
				pros::lcd::set_text(2, "Press again to toggle side.");

				lFDrive.move(master.get_analog(ANALOG_RIGHT_Y) + master.get_analog(ANALOG_RIGHT_X));
				lRDrive.move(master.get_analog(ANALOG_RIGHT_Y) + master.get_analog(ANALOG_RIGHT_X));
				rFDrive.move(master.get_analog(ANALOG_RIGHT_Y) - master.get_analog(ANALOG_RIGHT_X));
				rRDrive.move(master.get_analog(ANALOG_RIGHT_Y) - master.get_analog(ANALOG_RIGHT_X));
			break;
			case 4:
				pros::lcd::set_text(1, "Single Stick Arcade");
				pros::lcd::set_text(2, "Press again to toggle side.");

				lFDrive.move(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X));
				lRDrive.move(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X));
				rFDrive.move(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X));
				rRDrive.move(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X));
			break;
			default:
			break;
		}

		if (master.get_digital(DIGITAL_R1)) {
			claw.move_velocity(100);
			clawWasOpen = true;
		}
		else if (master.get_digital(DIGITAL_R2)) {
			clawWasOpen = false;
			claw.move_velocity(-100);
		}
		else if (!clawWasOpen) {
			claw.move_voltage(-12000);
		}
		else {
			claw.move_velocity(0);
		}

		if (master.get_digital(DIGITAL_L1)) {
			lift.move_velocity(200);
		}
		else if (master.get_digital(DIGITAL_L2)) {
			lift.move_velocity(-50);
		}
		else {
			lift.move_velocity(0);
		}

		driveMode = changeMode(driveMode);
		while (master.get_digital(DIGITAL_UP) || master.get_digital(DIGITAL_DOWN) || master.get_digital(DIGITAL_RIGHT) || master.get_digital(DIGITAL_LEFT)) {
			pros::delay(20);
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
	return mode;
}

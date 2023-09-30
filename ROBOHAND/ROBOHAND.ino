#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
SoftwareSerial mySerial(2, 3); // RX, TX

const double pi                     = 3.1415926535897932;
const double lever_1                = 100;
const double lever_2                = 83;
const double lever_3                = 84.5;
const int number_of_points          = 10;
const int min_delay                 = 1;
const int mid_delay                 = 10;
const int max_delay                 = 1000;
const double step_length            = 1;
const double rotate_step            = 5;

const int SERVOMIN                                     = 150; // This is the 'minimum' pulse length count (out of 4096)
const int SERVOMAX                                     = 600; // This is the 'maximum' pulse length count (out of 4096)
const int USMIN                                        = 600; // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
const int USMAX                                        = 2400; // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
const int SERVO_FREQ                                   = 50; // Analog servos run at ~50 Hz updates

const int servo[] = {1, 2, 3, 4, 5, 6};

struct half_angles {
    double gamma;
    double epsilon;
};

struct angles {
    double alpha;
    double beta;
    double delta;
    double gamma;
    double epsilon;
    double zeta;
};

struct axis {
    double x;
    double y;
    double z;
};

half_angles current_angles = {0, 0};
axis current_axis = {5, 0, 4};

int pulseWidth(int angle) {
    int pulse_wide, analog_value;
    pulse_wide     = map(angle, 0, 180, USMIN, USMAX);
    analog_value = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);
    return analog_value;
}

double RAD_TO_ANG(double radian) {
    return radian * 180 / pi;
}

void ninty_degree() {
    pwm.setPWM(servo[0], 0, pulseWidth(90));
    delay(min_delay);
    pwm.setPWM(servo[1], 0, pulseWidth(90));
    delay(min_delay);
    pwm.setPWM(servo[2], 0, pulseWidth(90));
    delay(min_delay);
        pwm.setPWM(servo[3], 0, pulseWidth(0));
    delay(min_delay);
    pwm.setPWM(servo[4], 0, pulseWidth(0));
    delay(min_delay);
    pwm.setPWM(servo[5], 0, pulseWidth(0));
    delay(min_delay);
}

void set_start_position(const axis &new_axis, const half_angles &new_angles) {
    angles ang;
    double c = sqrt(pow(new_axis.x, 2) + pow(new_axis.y, 2) + pow(new_axis.z, 2));
    double half_c = sqrt(pow(new_axis.x, 2) + pow(new_axis.y, 2));
    ang.alpha = RAD_TO_ANG(atan2(new_axis.z, half_c)) + RAD_TO_ANG(acos((pow(c, 2) + pow(lever_1, 2) - pow(lever_2, 2)) / (2 * lever_1 * c)));
    ang.beta = RAD_TO_ANG(acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(c, 2)) / (2 * lever_1 * lever_2)));
    ang.delta = 180 - RAD_TO_ANG(atan2(new_axis.z, lever_3));
    ang.zeta = ((new_axis.x * new_axis.y) >= double(0)) ? RAD_TO_ANG(atan2(new_axis.y, new_axis.x)) : (180 - RAD_TO_ANG(atan2(new_axis.y, new_axis.x)));
    delay(min_delay);
    ang.zeta = RAD_TO_ANG(atan2(new_axis.y, new_axis.x));
    pwm.setPWM(servo[0], 0, pulseWidth(ang.alpha));
    delay(min_delay);
    pwm.setPWM(servo[1], 0, pulseWidth(ang.beta));
    delay(min_delay);
    pwm.setPWM(servo[2], 0, pulseWidth(ang.delta));
    delay(min_delay);
    pwm.setPWM(servo[3], 0, pulseWidth(current_angles.gamma));
    delay(min_delay);
    pwm.setPWM(servo[4], 0, pulseWidth(current_angles.epsilon));
    delay(min_delay);
    pwm.setPWM(servo[5], 0, pulseWidth(ang.zeta));
    delay(min_delay);
}

void find_angles_and_move(const axis &start_axis, const axis &end_axis, const half_angles &start_angles, const half_angles &end_angles) {
    angles ang;
    axis new_axis = start_axis;
    ang.epsilon = start_angles.epsilon;
    ang.gamma = start_angles.gamma;
    ang.zeta = RAD_TO_ANG(atan2(start_axis.y, start_axis.x));
    double delta_x = (start_axis.x - end_axis.x) / number_of_points;
    double delta_y = (start_axis.y - end_axis.y) / number_of_points;
    double delta_z = (start_axis.z - end_axis.z) / number_of_points;
    double delta_epsilon = (end_angles.epsilon - start_angles.epsilon) / number_of_points;
    double delta_gamma = (end_angles.gamma - start_angles.gamma) / number_of_points;
    for (int a = 0; a <= number_of_points; a++) {
        new_axis.x += delta_x;
        new_axis.y += delta_y;
        new_axis.z += delta_z;
        double c = sqrt(pow(new_axis.x, 2) + pow(new_axis.y, 2) + pow(new_axis.z, 2));
        double half_c = sqrt(pow(new_axis.x, 2) + pow(new_axis.y, 2));
        if ((new_axis.x >= 0) && (new_axis.y >= 0)) {
            ang.alpha = RAD_TO_ANG(atan2(new_axis.z, half_c)) + RAD_TO_ANG(acos((pow(c, 2) + pow(lever_1, 2) - pow(lever_2, 2)) / (2 * lever_1 * c)));
            ang.beta = RAD_TO_ANG(acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(c, 2)) / (2 * lever_1 * lever_2)));
            ang.delta = 180 - RAD_TO_ANG(atan2(new_axis.z, lever_3));
            ang.zeta = RAD_TO_ANG(atan2(new_axis.y, new_axis.x));
        } else if ((new_axis.x < 0) && (new_axis.y >= 0))    {
            ang.alpha = RAD_TO_ANG(atan2(new_axis.z, half_c)) + RAD_TO_ANG(acos((pow(c, 2) + pow(lever_1, 2) - pow(lever_2, 2)) / (2 * lever_1 * c)));
            ang.beta = RAD_TO_ANG(acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(c, 2)) / (2 * lever_1 * lever_2)));
            ang.delta = 180 - RAD_TO_ANG(atan2(new_axis.z, lever_3));
            ang.zeta = 180 - RAD_TO_ANG(atan2(new_axis.y, new_axis.x));
        } else if ((new_axis.x >= 0) && (new_axis.y < 0)) {
            ang.alpha = 180 - RAD_TO_ANG(atan2(new_axis.z, half_c)) + RAD_TO_ANG(acos((pow(c, 2) + pow(lever_1, 2) - pow(lever_2, 2)) / (2 * lever_1 * c)));
            ang.beta = 180 - RAD_TO_ANG(acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(c, 2)) / (2 * lever_1 * lever_2)));
            ang.delta = RAD_TO_ANG(atan2(new_axis.z, lever_3));
            ang.zeta = 180 - RAD_TO_ANG(atan2(new_axis.y, new_axis.x));
        } else if ((new_axis.x < 0) && (new_axis.y < 0)) {
            ang.alpha = 180 - RAD_TO_ANG(atan2(new_axis.z, half_c)) + RAD_TO_ANG(acos((pow(c, 2) + pow(lever_1, 2) - pow(lever_2, 2)) / (2 * lever_1 * c)));
            ang.beta = 180 - RAD_TO_ANG(acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(c, 2)) / (2 * lever_1 * lever_2)));
            ang.delta = RAD_TO_ANG(atan2(new_axis.z, lever_3));
            ang.zeta = RAD_TO_ANG(atan2(new_axis.y, new_axis.x));
        } else {
            Serial.print("Too much length\n");
        }
        ang.epsilon += delta_epsilon;
        ang.gamma += delta_gamma;
        delay(min_delay);
        pwm.setPWM(servo[0], 0, pulseWidth(ang.alpha));
        delay(min_delay);
        pwm.setPWM(servo[1], 0, pulseWidth(ang.beta));
        delay(min_delay);
        pwm.setPWM(servo[2], 0, pulseWidth(ang.delta));
        delay(min_delay);
        pwm.setPWM(servo[3], 0, pulseWidth(ang.gamma));
        delay(min_delay);
        pwm.setPWM(servo[4], 0, pulseWidth(ang.epsilon));
        delay(min_delay);
        pwm.setPWM(servo[5], 0, pulseWidth(ang.zeta + a * delta_zeta));
        delay(min_delay);
        delay(mid_delay);
    }
}

void setup() {
    Serial.begin(57600);
    while (!Serial) {
        ; // ожидаем подключения к последовательному порту. Необходимо только для Leonardo
    }
    mySerial.begin(9600);
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(SERVO_FREQ);    // Analog servos run at ~50 Hz updates

}

void loop() {
    ninty_degree();
    delay(max_delay);
    set_start_position(current_axis, current_angles);
    delay(max_delay);
    while (1) {
        int parcel = 0;
        if (mySerial.available()) {
            parcel = mySerial.read();
            Serial.write("Parcel is " , parcel);
        }
        switch (parcel) {
            case 1:
                current_axis.x += step_length;
                break;
            case 2:
                current_axis.x -= step_length;
                break;
            case 3:
                current_axis.y += step_length;
                break;
            case 4:
                current_axis.y -= step_length;
                break;
            case 5:
                current_axis.z += step_length;
                break;
            case 6:
                current_axis.z -= step_length;
                break;
            case 7:
                current_angles.epsilon += rotate_step;
                break;
            case 8:
                current_angles.epsilon -= rotate_step;
                break;
            case 9:
                current_angles.gamma += rotate_step;
                break;
            case 10:
                current_angles.gamma -= rotate_step;
                break;
            default:
                Serial.write("Unapropriate parcel\n");
        }

    }
}

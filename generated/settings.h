/* This file has been generated from /home/manish/Paprazzi/paparazzi/var/Microjet/settings_modules.xml /home/manish/Paprazzi/paparazzi/conf/settings/estimation/infrared.xml /home/manish/Paprazzi/paparazzi/conf/settings/control/ctl_basic.xml /home/manish/Paprazzi/paparazzi/conf/settings/fixedwing_basic.xml /home/manish/Paprazzi/paparazzi/var/Microjet/settings_telemetry.xml */
/* Please DO NOT EDIT */

#ifndef SETTINGS_H
#define SETTINGS_H

#define RCSettings(mode_changed) { \
}
/******************TODO**********************
#include "autopilot.h"
#include "guidance/guidance_v.h"
#include "inter_mcu.h"
#include "stabilization/stabilization_attitude.h"
#include "subsystems/gps.h"
#include "subsystems/nav.h"
#include "subsystems/sensors/infrared.h"
*********************************************/
#include "generated/modules.h"
#include "generated/periodic_telemetry.h"

#define DlSetting(_idx, _value) { \
  switch (_idx) { \
    case 0: telemetry_mode_Ap = _value; break;\
    case 1: telemetry_mode_Fbw = _value; break;\
    case 2: flight_altitude = _value; break;\
    case 3: nav_course = _value; break;\
    case 4: nav_IncreaseShift( _value ); _value = nav_shift; break;\
    case 5: autopilot_ResetFlightTimeAndLaunch( _value ); _value = estimator_flight_time; break;\
    case 6: nav_SetNavRadius( _value ); _value = nav_radius; break;\
    case 7: pprz_mode = _value; break;\
    case 8: launch = _value; break;\
    case 9: kill_throttle = _value; break;\
    case 10: gps_Reset( _value ); _value = gps.reset; break;\
    case 11: ap_state->command_roll_trim = _value; break;\
    case 12: ap_state->command_pitch_trim = _value; break;\
    case 13: h_ctl_roll_pgain = _value; break;\
    case 14: h_ctl_roll_max_setpoint = _value; break;\
    case 15: h_ctl_pitch_pgain = _value; break;\
    case 16: h_ctl_pitch_dgain = _value; break;\
    case 17: h_ctl_elevator_of_roll = _value; break;\
    case 18: h_ctl_aileron_of_throttle = _value; break;\
    case 19: h_ctl_roll_attitude_gain = _value; break;\
    case 20: h_ctl_roll_rate_gain = _value; break;\
    case 21: v_ctl_altitude_pgain = _value; break;\
    case 22: guidance_v_SetCruiseThrottle( _value ); _value = v_ctl_auto_throttle_cruise_throttle; break;\
    case 23: v_ctl_auto_throttle_pgain = _value; break;\
    case 24: v_ctl_auto_throttle_igain = _value; break;\
    case 25: v_ctl_auto_throttle_dgain = _value; break;\
    case 26: v_ctl_auto_throttle_climb_throttle_increment = _value; break;\
    case 27: v_ctl_auto_throttle_pitch_of_vz_pgain = _value; break;\
    case 28: v_ctl_auto_throttle_pitch_of_vz_dgain = _value; break;\
    case 29: v_ctl_auto_pitch_pgain = _value; break;\
    case 30: v_ctl_auto_pitch_igain = _value; break;\
    case 31: h_ctl_course_pgain = _value; break;\
    case 32: h_ctl_course_dgain = _value; break;\
    case 33: h_ctl_course_pre_bank_correction = _value; break;\
    case 34: nav_glide_pitch_trim = _value; break;\
    case 35: h_ctl_roll_slew = _value; break;\
    case 36: nav_radius = _value; break;\
    case 37: nav_course = _value; break;\
    case 38: nav_mode = _value; break;\
    case 39: nav_climb = _value; break;\
    case 40: fp_pitch = _value; break;\
    case 41: nav_IncreaseShift( _value ); _value = nav_shift; break;\
    case 42: nav_ground_speed_setpoint = _value; break;\
    case 43: nav_ground_speed_pgain = _value; break;\
    case 44: nav_survey_shift = _value; break;\
    case 45: infrared.roll_neutral = _value; break;\
    case 46: infrared.pitch_neutral = _value; break;\
    case 47: infrared.lateral_correction = _value; break;\
    case 48: infrared.longitudinal_correction = _value; break;\
    case 49: infrared.vertical_correction = _value; break;\
    case 50: infrared.correction_left = _value; break;\
    case 51: infrared.correction_right = _value; break;\
    case 52: infrared.correction_up = _value; break;\
    case 53: infrared.correction_down = _value; break;\
    case 54: openlog_periodic_2Hz_openlog_status = _value; break;\
    case 55: digital_cam_servo_servo_cam_ctrl_periodic_status = _value; break;\
    default: break;\
  }\
}
#define PeriodicSendDlValue(_trans, _dev) { \
  static uint8_t i;\
  float var;\
  if (i >= 56) i = 0;\
  switch (i) { \
    case 0: var = telemetry_mode_Ap; break;\
    case 1: var = telemetry_mode_Fbw; break;\
    case 2: var = flight_altitude; break;\
    case 3: var = nav_course; break;\
    case 4: var = nav_shift; break;\
    case 5: var = estimator_flight_time; break;\
    case 6: var = nav_radius; break;\
    case 7: var = pprz_mode; break;\
    case 8: var = launch; break;\
    case 9: var = kill_throttle; break;\
    case 10: var = gps.reset; break;\
    case 11: var = ap_state->command_roll_trim; break;\
    case 12: var = ap_state->command_pitch_trim; break;\
    case 13: var = h_ctl_roll_pgain; break;\
    case 14: var = h_ctl_roll_max_setpoint; break;\
    case 15: var = h_ctl_pitch_pgain; break;\
    case 16: var = h_ctl_pitch_dgain; break;\
    case 17: var = h_ctl_elevator_of_roll; break;\
    case 18: var = h_ctl_aileron_of_throttle; break;\
    case 19: var = h_ctl_roll_attitude_gain; break;\
    case 20: var = h_ctl_roll_rate_gain; break;\
    case 21: var = v_ctl_altitude_pgain; break;\
    case 22: var = v_ctl_auto_throttle_cruise_throttle; break;\
    case 23: var = v_ctl_auto_throttle_pgain; break;\
    case 24: var = v_ctl_auto_throttle_igain; break;\
    case 25: var = v_ctl_auto_throttle_dgain; break;\
    case 26: var = v_ctl_auto_throttle_climb_throttle_increment; break;\
    case 27: var = v_ctl_auto_throttle_pitch_of_vz_pgain; break;\
    case 28: var = v_ctl_auto_throttle_pitch_of_vz_dgain; break;\
    case 29: var = v_ctl_auto_pitch_pgain; break;\
    case 30: var = v_ctl_auto_pitch_igain; break;\
    case 31: var = h_ctl_course_pgain; break;\
    case 32: var = h_ctl_course_dgain; break;\
    case 33: var = h_ctl_course_pre_bank_correction; break;\
    case 34: var = nav_glide_pitch_trim; break;\
    case 35: var = h_ctl_roll_slew; break;\
    case 36: var = nav_radius; break;\
    case 37: var = nav_course; break;\
    case 38: var = nav_mode; break;\
    case 39: var = nav_climb; break;\
    case 40: var = fp_pitch; break;\
    case 41: var = nav_shift; break;\
    case 42: var = nav_ground_speed_setpoint; break;\
    case 43: var = nav_ground_speed_pgain; break;\
    case 44: var = nav_survey_shift; break;\
    case 45: var = infrared.roll_neutral; break;\
    case 46: var = infrared.pitch_neutral; break;\
    case 47: var = infrared.lateral_correction; break;\
    case 48: var = infrared.longitudinal_correction; break;\
    case 49: var = infrared.vertical_correction; break;\
    case 50: var = infrared.correction_left; break;\
    case 51: var = infrared.correction_right; break;\
    case 52: var = infrared.correction_up; break;\
    case 53: var = infrared.correction_down; break;\
    case 54: var = openlog_periodic_2Hz_openlog_status; break;\
    case 55: var = digital_cam_servo_servo_cam_ctrl_periodic_status; break;\
    default: var = 0.; break;\
  }\
  DOWNLINK_SEND_DL_VALUE(_trans, _dev, &i, &var);\
  i++;\
}
static inline float settings_get_value(uint8_t i) {
  switch (i) { \
    case 0: return telemetry_mode_Ap;
    case 1: return telemetry_mode_Fbw;
    case 2: return flight_altitude;
    case 3: return nav_course;
    case 4: return nav_shift;
    case 5: return estimator_flight_time;
    case 6: return nav_radius;
    case 7: return pprz_mode;
    case 8: return launch;
    case 9: return kill_throttle;
    case 10: return gps.reset;
    case 11: return ap_state->command_roll_trim;
    case 12: return ap_state->command_pitch_trim;
    case 13: return h_ctl_roll_pgain;
    case 14: return h_ctl_roll_max_setpoint;
    case 15: return h_ctl_pitch_pgain;
    case 16: return h_ctl_pitch_dgain;
    case 17: return h_ctl_elevator_of_roll;
    case 18: return h_ctl_aileron_of_throttle;
    case 19: return h_ctl_roll_attitude_gain;
    case 20: return h_ctl_roll_rate_gain;
    case 21: return v_ctl_altitude_pgain;
    case 22: return v_ctl_auto_throttle_cruise_throttle;
    case 23: return v_ctl_auto_throttle_pgain;
    case 24: return v_ctl_auto_throttle_igain;
    case 25: return v_ctl_auto_throttle_dgain;
    case 26: return v_ctl_auto_throttle_climb_throttle_increment;
    case 27: return v_ctl_auto_throttle_pitch_of_vz_pgain;
    case 28: return v_ctl_auto_throttle_pitch_of_vz_dgain;
    case 29: return v_ctl_auto_pitch_pgain;
    case 30: return v_ctl_auto_pitch_igain;
    case 31: return h_ctl_course_pgain;
    case 32: return h_ctl_course_dgain;
    case 33: return h_ctl_course_pre_bank_correction;
    case 34: return nav_glide_pitch_trim;
    case 35: return h_ctl_roll_slew;
    case 36: return nav_radius;
    case 37: return nav_course;
    case 38: return nav_mode;
    case 39: return nav_climb;
    case 40: return fp_pitch;
    case 41: return nav_shift;
    case 42: return nav_ground_speed_setpoint;
    case 43: return nav_ground_speed_pgain;
    case 44: return nav_survey_shift;
    case 45: return infrared.roll_neutral;
    case 46: return infrared.pitch_neutral;
    case 47: return infrared.lateral_correction;
    case 48: return infrared.longitudinal_correction;
    case 49: return infrared.vertical_correction;
    case 50: return infrared.correction_left;
    case 51: return infrared.correction_right;
    case 52: return infrared.correction_up;
    case 53: return infrared.correction_down;
    case 54: return openlog_periodic_2Hz_openlog_status;
    case 55: return digital_cam_servo_servo_cam_ctrl_periodic_status;
    default: return 0.;
    }
  }

/* Persistent Settings */
struct PersistentSettings {
};

extern struct PersistentSettings pers_settings;

static inline void persistent_settings_store( void ) {
}

static inline void persistent_settings_load( void ) {
};

#endif // SETTINGS_H

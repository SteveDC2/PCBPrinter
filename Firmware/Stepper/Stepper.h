/*
 * Stepper.h
 *
 *  Created on: Nov 22, 2016
 *      Author: Steve
 */

#ifndef STEPPER_STEPPER_H_
#define STEPPER_STEPPER_H_

/////////////////////////////////////////////////////////////////////////////////////
//      Stepping functions
/////////////////////////////////////////////////////////////////////////////////////
extern uint32_t HomeYStepPeriodFast;
extern uint32_t HomeYStepPeriodSlow;
extern uint32_t HomeXStepPeriodFast;
extern uint32_t HomeXStepPeriodSlow;
extern int32_t XAccelerationTime;

extern uint32_t StepsToRun;
extern uint8_t StepsRunning;
extern uint32_t StepPeriod;
extern uint8_t CheckLimits;

extern float XAcceleration;
extern int32_t XStepTracker;

extern uint8_t StepperDebug;

void Stepper_PrepareStepSettings(void);
void Stepper_StepperInterrupt(void);
void Stepper_StepperPulseInterrupt(void);
int32_t Stepper_GetNextStepPeriod();
void Stepper_MoveSteps(int32_t StepCount, uint8_t MotorToMove);
void Stepper_EnableSteppers(uint8_t State);
void Stepper_Home(uint32_t PeriodFast, uint32_t PeriodSlow, uint8_t LimitCheck, uint8_t MotorToMove, uint16_t TicksPermm);
void Stepper_ResetSteppers(void);
void Stepper_LoopMove(uint32_t Motor, uint32_t MaxTravel, uint32_t Period);
void Stepper_InitStepSegment(int32_t Period, int32_t RampDistance, int32_t RampTimeuS, int8_t UpDown);
void Stepper_RunInY();


#endif /* STEPPER_STEPPER_H_ */

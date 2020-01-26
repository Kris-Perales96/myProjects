#include "mbed.h"
#include "MMA8451Q.h"
#include "max6675.h"
#include <string>
#include <math.h>

#include "arm_common_tables.h"
#include "arm_const_structs.h"
#include "math_helper.h"
#include "arm_math.h"

#define MMA8451_I2C_ADDRESS (0x1d<<1)
#define SAMPLES 1024
#define SIZE_OF_FFT (SAMPLES * 2)
#define NUMBER_OF_TAPS 25
#define NUMBER_OF_TAPS_MAF 10
#define SAMPLING_FREQUENCY 1024.0

const float NOMOVEMENTTIMECONSTANT = 10.0;
const float APPROACHINGDANGEROUSLEVEL = 82.0;
const float TEMPDANGEROUSLEVEL = 87.0;
const int LOWDANGERLEVELHR = 40;
const int UPPERDANGERLEVELHR = 182;
const int APPROACHINGDANGERLEVELLOWER = 55;
const int APPROACHINGDANGERLEVELUPPER = 170;


float32_t HRIn[SIZE_OF_FFT];
float32_t HROut[SAMPLES];
float32_t HRIFFT[SAMPLES / 2];

////////////////////////////////////////////// Used For the FIR Low Pass filter and Moving Average filter where cutoff is 16 Hz and sample frequency of 1024
static float32_t FIRStateMAF[SAMPLES + NUMBER_OF_TAPS_MAF - 1];
const float32_t FIRCoeffsMAF[NUMBER_OF_TAPS_MAF] = { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f };

static float32_t firStateF32[SAMPLES + NUMBER_OF_TAPS - 1];
const float32_t firCoeffs32[NUMBER_OF_TAPS] = { 
     0.0050f,    0.0062f,    0.0096f,    0.0150f,    0.0222f,    0.0310f,    0.0406f,    0.0504f,    0.0598f,    0.0679f,    0.0743f,    0.0783f,    0.0797f,
     0.0783f,    0.0743f,    0.0679f,    0.0598f,    0.0504f,    0.0406f,    0.0310f,    0.0222f,    0.0150f,    0.0096f,    0.0062f,    0.0050f };

MMA8451Q accel(PTE25, PTE24, MMA8451_I2C_ADDRESS);
Serial pc(USBTX, USBRX);
DigitalOut led1(LED1);
SPI thermocouple(PTD6,PTD7,PTD5);
max6675 maxForThermocouple(thermocouple, PTD6);
PwmOut vibratingMotor(PTC9);
AnalogIn ain(A0);
Serial xbee(PTE0, PTE1);

Timer zeroReadingForAccelerometerTime;
Timer samplingTimer;
Timer getBPMTimer;
Timer analyze;
Timer grabBPM;

Timer invalidDataForHR;
Timer invalidDataForTemp;

int main()

{ 
    pc.baud(115200);
    led1 = !led1;
    
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// THRESHOLD VALUES ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    float SAMPLETIMEVALUE = 1.0 / SAMPLING_FREQUENCY;
    float ANALYZEVALUE = 1.0;
    float BPMTIMEVALUE = 1.0;
    
    float invalidDataTemp = 0;          //////////////////// May need to be changed after inspection later which is plugging in the temp sensor wrongly and seeing what is read by the HR sensor with not signal
    float invalidDataHR = 240;
    
    float timeNeededToSignifySensorNotWorking = 30.0;
    bool HRSensorNotWorking = false,
         tempSensorNotWorking = false;
    int sensorNotWorkingSignal = 0;

    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// ACCELEROMETER VARIABLES /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int numberOfAccelerometerToMaintain = 2;
    int indexForMagnitudeAccelerometer = 0;
    bool toggleForMagnitudeIndex = false;
    float accelerometerXReading = 0.0, accelerometerYReading = 0.0, accelerometerZReading = 0.0;
    float magnitudeOfAccelerometer[numberOfAccelerometerToMaintain];
    
    for(int i = 0; i < numberOfAccelerometerToMaintain; ++i)
        {
            magnitudeOfAccelerometer[i] = 0;
        }

    int zeroReadingForAccelerometerFlag = 0, 
        timerStartedFlag = 0;
    int userHasNotMovedForXTime = 0;
    

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////  Temperature VARIABLES /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float temperatureOfTheRoom;
    bool temperatureApproachingDangerLevel = false,
         temperatureAtDangerLevel = false,
         motorIsActive = false;                    //This is going to be used in sending wireless warnings or all clears
    float approachingDangerousPeriod = 3.0,
          approachingDangerousPeriodHR = 2.0,
          DangerousLevelPeriod = 1.0,
          dutyCycleForApproach = 0.5,
          dutyCycleForApproachHR = 0.75,
          dutyCycleForDangerLevel = 1.0; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////  Heart Rate Variables   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float signal = 0.0;
    bool toggleForBPMIndex = false,
         motorActiveForHeartRate = false,
         BPMAtDangerLevel = false,
         BPMApproachingDangerLevel = false;
    int BPMIndex = 0;
    bool analysisTimerStarted = false,
         samplingTimerStarted = false,
         grabBPMTimerStarted = false;
    float timeForBPMScaled = 0.0;
    int BPM = 0;

        
    int sampleIndex = 0;
    uint32_t indexForFFT = 0;
    
    int j = 0,
        motorIndicator = 0;
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// FIR FILTER INSTANTIATION AND SETUP //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    arm_fir_instance_f32 firLPF;
    arm_fir_init_f32(&firLPF, NUMBER_OF_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], SAMPLES);
    arm_fir_instance_f32 firMAF;
    arm_fir_init_f32(&firMAF, NUMBER_OF_TAPS_MAF, (float32_t *)&FIRCoeffsMAF[0], &FIRStateMAF[0], SAMPLES);    


//////////////////////////// DATA STORED AND VARIABLES FOR OUTPUT TXT /////////////////////////////////////


    while (true)

    {
            
        if(!samplingTimerStarted)
            {
                samplingTimer.start();
                samplingTimerStarted = true;
            }
        if(!analysisTimerStarted)
            {
                analysisTimerStarted = true;
                analyze.start();
            
            }
            
            
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////  Heart Rate Analysis    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
            if (samplingTimer.read() >= SAMPLETIMEVALUE)
                {
                    samplingTimerStarted = false;
                    signal = ain.read();
                    HRIn[sampleIndex] = signal;
                    HRIn[sampleIndex + 1] = 0.0;
                    sampleIndex += 2;
                    samplingTimer.reset();
                }
               
            
                    
            if(analyze.read() >= ANALYZEVALUE)
                {
                    
                    if(!grabBPMTimerStarted)
                        {
                            grabBPM.start();
                            grabBPMTimerStarted = true;
                        }
                    if(grabBPM.read() >= BPMTIMEVALUE)
                        {
                            arm_fir_f32(&firLPF, HRIn, HRIn, SAMPLES);
                            arm_fir_f32(&firMAF, HRIn, HRIn, SAMPLES);
                            
                            arm_cfft_f32(&arm_cfft_sR_f32_len1024, HRIn, 0, 1);
                            arm_cmplx_mag_f32(HRIn, HROut, SAMPLES);
                            arm_cfft_f32(&arm_cfft_sR_f32_len512, HROut, 1, 1);
                            arm_cmplx_mag_f32(HROut, HRIFFT, SAMPLES/2);
                            arm_max_f32(HRIFFT, SAMPLES / 2, &timeForBPMScaled, &indexForFFT);
                            
                            BPM = (int)((timeForBPMScaled) * 60.0);
                            
                            timeForBPMScaled = 0.0;
                            sampleIndex = 0;
                            
                            if((LOWDANGERLEVELHR < BPM && BPM <= APPROACHINGDANGERLEVELLOWER) || (APPROACHINGDANGERLEVELUPPER <= BPM && BPM < UPPERDANGERLEVELHR))
                                {
                                    motorActiveForHeartRate = true;
                                    vibratingMotor.period(approachingDangerousPeriodHR);
                                    vibratingMotor = dutyCycleForApproachHR;
                                    BPMApproachingDangerLevel = true;  
                                    BPMAtDangerLevel = false;
                                }
                            else if (BPM <= LOWDANGERLEVELHR || BPM >= UPPERDANGERLEVELHR)
                                {
                                    BPMApproachingDangerLevel = false;
                                    BPMAtDangerLevel = true;
                                    motorActiveForHeartRate = true;
                                    vibratingMotor.period(DangerousLevelPeriod);
                                    vibratingMotor = dutyCycleForDangerLevel;
                                }
                            else if(BPM > APPROACHINGDANGERLEVELLOWER && BPM < APPROACHINGDANGERLEVELUPPER)
                                {
                                    motorActiveForHeartRate = false;
                                    vibratingMotor = 0.0;
                                    BPMApproachingDangerLevel = false;
                                    BPMAtDangerLevel = false;
                                }
                            
                            
                            if(!toggleForBPMIndex)
                                BPMIndex = 0;
                            else
                                BPMIndex = 1;
                            
                            grabBPM.reset();
                            grabBPMTimerStarted = false;
                            
                            if(BPM >= invalidDataHR && !HRSensorNotWorking)
                                {
                                    invalidDataForHR.start();
                                    HRSensorNotWorking = true;
                                }
                            else if (BPM < invalidDataHR)
                                {
                                    invalidDataForHR.reset();
                                    HRSensorNotWorking = false;
                                } 
                        }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Temperature ANALYSIS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
                temperatureOfTheRoom = (maxForThermocouple.read_temp()*(9.0/5.0)) + 32.0;
            
                if(temperatureOfTheRoom >= APPROACHINGDANGEROUSLEVEL && temperatureOfTheRoom < TEMPDANGEROUSLEVEL)
                    {   
                        temperatureApproachingDangerLevel = true;
                        temperatureAtDangerLevel = false;
                    }
                else if (temperatureOfTheRoom >= TEMPDANGEROUSLEVEL)
                    {
                        temperatureApproachingDangerLevel = false;
                        temperatureAtDangerLevel = true;
                    }
                else
                    {
                        temperatureApproachingDangerLevel = false,
                        temperatureAtDangerLevel = false;
                        motorIsActive = false;
                        vibratingMotor = 0.0;
                    }
                
                if (temperatureApproachingDangerLevel)
                    {
                        motorIsActive = true;
                        vibratingMotor.period(approachingDangerousPeriod);
                        vibratingMotor = dutyCycleForApproach;                   //implement the duty cycle for motor
                    }
                if (temperatureAtDangerLevel)
                    {
                        motorIsActive = true;
                        vibratingMotor.period(DangerousLevelPeriod);
                        vibratingMotor = dutyCycleForDangerLevel;                  //implement the duty cycle for motor
                    }
                    
                if(temperatureOfTheRoom <= invalidDataTemp && !tempSensorNotWorking)
                    {
                        invalidDataForTemp.start();
                        tempSensorNotWorking = true;
                    }
                else if (temperatureOfTheRoom > invalidDataTemp)
                    {
                        invalidDataForTemp.reset();
                        tempSensorNotWorking = false;
                    }
                    
 
            
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// ACCELEROMETER ANALYSIS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                accelerometerXReading = accel.getAccX();
                accelerometerYReading = accel.getAccY();
                accelerometerZReading = accel.getAccZ();
                magnitudeOfAccelerometer[indexForMagnitudeAccelerometer] = sqrt((accelerometerXReading * accelerometerXReading + accelerometerYReading * accelerometerYReading + accelerometerZReading * accelerometerZReading )); // acceleration to be used in detecting a fall
                float changeInAcceleration = abs((magnitudeOfAccelerometer[0] - magnitudeOfAccelerometer[1]));
        
                if(changeInAcceleration < 0.15 && (timerStartedFlag == 0))         //Accelerometer reads zero, firefighter may not be moving
                    {
                        zeroReadingForAccelerometerFlag = 1;
                        timerStartedFlag = 1;
                        zeroReadingForAccelerometerTime.start();
                    }
                          
                if(zeroReadingForAccelerometerTime.read() >= NOMOVEMENTTIMECONSTANT)    //Firefighter hasn't moved for x amount of time
                    {
                        userHasNotMovedForXTime = 1;
                    }
                
                if(changeInAcceleration >= 0.15) // Resetting flags for zero reading, timer, and reset of the timer
                    {
                        zeroReadingForAccelerometerTime.reset();
                        userHasNotMovedForXTime = 0;
                        timerStartedFlag = 0;
                        zeroReadingForAccelerometerFlag = 0;
                    }
                    
                if(changeInAcceleration <= 0.05)
                    {
                        
                        changeInAcceleration = 0.0;
                    }  
            
                toggleForMagnitudeIndex = !toggleForMagnitudeIndex;
            
                if(toggleForMagnitudeIndex == true)
                    indexForMagnitudeAccelerometer = 1;
                else
                    indexForMagnitudeAccelerometer = 0;
             
                if(motorActiveForHeartRate)
                    motorIndicator = 2;
                else if (motorIsActive)
                    motorIndicator = 1;
                else if(motorActiveForHeartRate && motorIsActive)
                    motorIndicator = 3;
                else
                    motorIndicator = 0;
                    
                if(invalidDataForHR.read() >= timeNeededToSignifySensorNotWorking)
                    {
                        sensorNotWorkingSignal = 2; // Heart rate sensor alone not reading valid data
                        BPM = 10000;
                    }
                else if(invalidDataForTemp.read() >= timeNeededToSignifySensorNotWorking)
                    {
                        sensorNotWorkingSignal = 1; // Temp sensor alone not reading valid data
                        temperatureOfTheRoom = 10000;
                    }
                else if(invalidDataForHR.read() >= timeNeededToSignifySensorNotWorking && invalidDataForTemp.read() >= timeNeededToSignifySensorNotWorking)
                    {
                        sensorNotWorkingSignal = 3; // both sensors not reading valid data
                        BPM = 10000; 
                        temperatureOfTheRoom = 10000;
                    }
                else
                    {
                        sensorNotWorkingSignal = 0; // all clear
                    }
                    

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Xbee Communication //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        
                printf("%i", j);
                printf(" %f", temperatureOfTheRoom);
                printf(" %i", userHasNotMovedForXTime);
                printf(" %f", changeInAcceleration);
                printf(" %i", BPM);
                printf(" %i\n", motorIndicator);
                //printf("The sensor warning signal is: %i\n", sensorNotWorkingSignal);
        
                j++;

                analysisTimerStarted = false;
                analyze.reset();
            }
        }  
 }

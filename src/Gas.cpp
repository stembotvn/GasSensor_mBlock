#include "Arduino.h"
#include "Gas.h"

Gas::Gas(/* args */)
{
}

float Gas::MQResistanceCalculation(int raw_adc){
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));

}
/////////////////
float Gas::MQCalibration(int mq_pin){
int i;
  float val=0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value
  val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro    
  Ro = val;                                    
  return val;        

}
////////////////////////
 float Gas::MQRead(int mq_pin){
int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
 
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;  
    }
//////////////////////////
long Gas::MQGetGasPercentage(float rs_ro_ratio, int gas_id){
if ( gas_id == GAS_LPG ) {
     return MQGetPercentage(rs_ro_ratio,LPGCurve);
  } else if ( gas_id == GAS_CO ) {
     return MQGetPercentage(rs_ro_ratio,COCurve);
  } else if ( gas_id == GAS_SMOKE ) {
     return MQGetPercentage(rs_ro_ratio,SmokeCurve);
  }    
 
  return 0;
}
//////////////////
long  Gas::MQGetPercentage(float rs_ro_ratio, float *pcurve){
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));

}
///////////////////////
long  Gas::MQGetPPM(int mq_pin,int  gas_id){

return MQGetGasPercentage(MQRead(mq_pin)/Ro,gas_id);   
 
}



#define _XRAY_TUBE_C

#include "application.h"
#include "xray_tube.h"
#include "Protocol/protocol.h" 

#define FAN_ON FAN_Clear()
#define FAN_OFF FAN_Set()



/*
 * Lookup Table 
|	71	|	0	|
|	78	|	5	|
|	85	|	10	|
|	92	|	15	|
|	100	|	20	|
|	107	|	25	|
|	114	|	30	|
|	121	|	35	|
|	127	|	40	|
|	134	|	45	|
|	140	|	50	|
|	146	|	55	|
|	151	|	60	|
|	156	|	65	|
|	161	|	70	|
|	166	|	75	|
|	170	|	80	|
|	174	|	85	|
|	178	|	90	|
|	181	|	95	|
|	184	|	100	|
|	187	|	105	|
 */
void XrayTubeInit(void){
    FAN_OFF;
    
    SystemStatusRegister.fan_stat = 0;
    SystemStatusRegister.fan_forced = 0;
    
    ADC0_Enable();
    ADC0_ConversionStart();    
}

typedef struct{
    unsigned char sensor;
    unsigned char perc;
}lookupT;


static const lookupT sensToPerc[] = {
    {71,0}, //!< 0°C
    {78,0}, //!< 5°C
    {85,0}, //!< 10°C
    {92,0}, //!< 15°C
    {100,0},//!< 20°C
    {107,6},//!< 25°C
    {114,12},//!< 30°C
    {121,18},//!< 35°C
    {127,24},//!< 40°C
    {134,30},//!< 45°C
    {140,36},//!< 50°C
    {146,42},//!< 55°C
    {151,48},//!< 60°C
    {156,54},//!< 65°C
    {161,60},//!< 70°C
    {166,66},//!< 75°C
    {170,72},//!< 80°C
    {174,78},//!< 85°C
    {178,84},//!< 90°C
    {181,90},//!< 95°C
    {184,96},//!< 100°C
    {187,100}//!< 105°C
               
};

static unsigned char analogToPerc(unsigned short val){
    
    // Trasformazione da 16 bit a 8 bit 
    unsigned char sensor = (unsigned char) (val >> 8);
    
    if(sensor <= 71 ) return 0;
    if(sensor >= 187 ) return 100;
    
    for(int i = 1; i < sizeof(sensToPerc) / sizeof(lookupT); i++){
        if(sensor < sensToPerc[i].sensor) return sensToPerc[i-1].perc;
    }
    
    return 0;    
}

void XrayTubeLoop(void){
    static int seq = 0;
    unsigned short sens;
    static unsigned char t_bulb = 0;
    static unsigned char t_stator = 0;
    
    // Selezione lettura alternata dei due sensori di temperatura
    switch(seq){
        case 0: // Selezione canale STATORE (AIN2)
            ADC0_ChannelSelect(ADC_POSINPUT_AIN2,ADC_NEGINPUT_GND );
            seq++;
            break;
        case 1: // lettura canale STATORE (AIN2) e selezione canale BULB (AIN3)
            sens = ADC0_ConversionResultGet();
            ADC0_ChannelSelect(ADC_POSINPUT_AIN3,ADC_NEGINPUT_GND );
            t_stator = analogToPerc(sens);
            seq++;
            break;
        case 2: // lettura canale BULB (AIN3) e Selezione canale STATORE (AIN2)
            sens = ADC0_ConversionResultGet();
            ADC0_ChannelSelect(ADC_POSINPUT_AIN2,ADC_NEGINPUT_GND );
            t_bulb = analogToPerc(sens);
            seq++;
            break;
            
        case 3: // Valutazione e azione   
            
            // Scrittura sul protocollo seriale del valore dei sensori
            TubeStatusRegister.bulb = t_bulb;
            TubeStatusRegister.stator = t_stator;
            encodeStatusRegister(&TubeStatusRegister);
            
            // If the Fan is forced, the FAN is activated
            if(SystemStatusRegister.fan_forced){
                FAN_ON;
                SystemStatusRegister.fan_stat = 1;
                encodeStatusRegister(&SystemStatusRegister);
            }else  if((t_bulb > 50) || (t_stator > 50)){ 
                FAN_ON;
                SystemStatusRegister.fan_stat = 1;
                encodeStatusRegister(&SystemStatusRegister);
            }else if ((t_bulb < 30) && (t_stator < 30)){ 
                FAN_OFF;
                SystemStatusRegister.fan_stat = 0;
                encodeStatusRegister(&SystemStatusRegister);
            }
            
            seq = 1; // Ripete
            break;
    }
    
   
}

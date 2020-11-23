
int aimantPIN = XX //PIN digital 0/5V
pinMode(aimantPIN,OUTPUT);

void PICK_UP()
{
    void SERVO_Enable(uint8_t id){                          //activation bras
    __Robus__.enableServo(id);
    }
    void SERVO_SetAngle(uint8_t id, uint8_t angle){         // descendre le bras
    __Robus__.setAngleServo(id, angle);
    }
     digitalWrite(aimantPIN, HIGH);                         //shenanigan d'aimant 
    delay(1000);
    void SERVO_SetAngle(uint8_t id, uint8_t angle){         //monter le bras
    __Robus__.setAngleServo(id, angle);
    }
    void SERVO_SetAngle(uint8_t id, uint8_t angle){         //descendre le bars
    __Robus__.setAngleServo(id, angle);
    }
    digitalWrite(aimantPIN, LOW);                           //shenanigan d'aimant
     delay(1000);
    void SERVO_SetAngle(uint8_t id, uint8_t angle){         //monter le bras
    __Robus__.setAngleServo(id, angle);
    }
    void SERVO_Disable(uint8_t id){                         //désactiver le bras
    __Robus__.disableServo(id);
    }
}
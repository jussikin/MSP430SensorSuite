env = Environment()

SRCNULL    =  ['main.c','nullsensor.c','NRF24L01p.c']
SRCMSP     =  ['main.c','mspsensor.c','NRF24L01p.c']
SRCONEWIRE =  ['main.c','onewire.c','onewiresensor.c','NRF24L01p.c']
SRCSWITCH  =  ['main.c','switchsensor.c','NRF24L01p.c']
SRCANALOG  =  ['main.c','analogsensor.c','NRF24L01p.c']
SRCCOUNTER  = ['main.c','countersensor.c','NRF24L01p.c']

MCU = "msp430g2432"
CFLAGS = ["-g", "-Os", "-mmcu=%(MCU)s" % {"MCU": MCU}, "-Wall", "-Wextra"]
LDFLAGS = "-mmcu=%(MCU)s" % {"MCU": MCU}
CC       = "msp430-gcc"
LD       = "msp430-ld"
AR       = "msp430-ar"
AS       = "msp430-gcc"
GASP     = "msp430-gasp"
NM       = "msp430-nm"
OBJCOPY  = "msp430-objcopy"
RANLIB   = "msp430-ranlib"
STRIP    = "msp430-strip"
SIZE     = "msp430-size"
READELF  = "msp430-readelf"

#defaults
ID=0
TYPE=0
INTERVAL=30

import os
import SCons.Script
import sys

for key, value in ARGLIST:
	if key == 'ID':
		ID=value
	if key == 'UDP':
		TYPE=TYPE+0b00100000
	if key == 'INTERVAL':
		INTERVAL = value

if ('1wire' in COMMAND_LINE_TARGETS):
	TYPE=TYPE+0b01000000
if ('msp' in COMMAND_LINE_TARGETS):
	TYPE=TYPE+0b00001000
if ('switch' in COMMAND_LINE_TARGETS):
	TYPE=TYPE+0b00000100
if ('analog' in COMMAND_LINE_TARGETS):
	TYPE=TYPE+0b00000010
if ('counter' in COMMAND_LINE_TARGETS):
	TYPE=TYPE+0b00000001


cppdefines = []
#Define the ID code for sensor
cppdefines.append("IDCODE={id}".format(id=ID))
#Number of seconds to sleep
cppdefines.append("ICOUNTMAX={interval}".format(interval=INTERVAL))
#Number of oscillations per secods
cppdefines.append("TRIGGER="+"12000")
#Amount of correction 
cppdefines.append("CORRECTION="+"0")
#Type code of the sensor 
cppdefines.append("TYPE={type}".format(type=TYPE))

env = Environment(ENV = os.environ, CC=CC, CFLAGS=CFLAGS, LINKFLAGS=LDFLAGS,CPPDEFINES = cppdefines)
mspTempSensor     = env.Program("MspSensor.elf", SRCMSP)
oneWireTempSensor = env.Program("oneWireTempSensor.elf",SRCONEWIRE)
nullSensor = env.Program("nullSensor.elf",SRCNULL)
switchSensor = env.Program("switchSensor.elf",SRCSWITCH)
analogSensor = env.Program("analogSensor.elf",SRCANALOG)
counterSensor = env.Program("counterSensor.elf",SRCCOUNTER)

env.Alias('1wire', "oneWireTempSensor.elf")
env.Alias('msp', "MspSensor.elf")
env.Alias('null',"nullSensor.elf")
env.Alias('switch',"switchSensor.elf")
env.Alias('analog',"analogSensor.elf")
env.Alias('counter',"counterSensor.elf")

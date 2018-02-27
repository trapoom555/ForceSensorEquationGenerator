import numpy as np
import matplotlib.pyplot as plt
from decimal import *
getcontext().prec = 10
Rcon = Decimal(input('Pull-down Resistance (ohm) :'))
Vs = Decimal(input('Voltage Source (Volt) :'))
times = int(input('Number of data set :'))
F_list = [] # x axis & input x
arduinoVoltage = [] # input y
inverseRf_list = [] # y axis
k = [] #array of Decimal ones

for i in range(0,times) :
    F_list.append(Decimal(input('Force %s :'%(i+1))))
    arduinoVoltage.append(Decimal(input('readableVoltage %s :'%(i+1)))*5/1024)
#print(F_list)
#print(arduinoVoltage)
for i in range(0,times) :
    inverseRf_list.append(arduinoVoltage[i]/(Rcon*(Vs-arduinoVoltage[i])))
print(inverseRf_list)
for i in range(0,len(F_list)) :
    k.append(Decimal(1))
A = np.vstack([F_list,k]).T
#print(A)
m,c = np.linalg.lstsq(A,inverseRf_list)[0]
#print(Decimal(m),Decimal(c))
print('-------------------------------------------')
print('1/Rf = m(F) + c ; m = %.3E c = %.3E' %(Decimal(m),Decimal(c)))
print('-------------------------------------------')
print('1/(%.3E((%.3E/Vr)-1)) - (%.3E)'%((Decimal(m)*Decimal(Rcon)),(Decimal(Vs)*Decimal(1024)/Decimal(5)),(Decimal(c)/Decimal(m))))

def toForce(x) :
    return 1/((Decimal(m)*Decimal(Rcon))*(((Decimal(Vs) * Decimal(1024) / Decimal(5))/Decimal(x)) - 1)) - (Decimal(c)/Decimal(m))

for i in range(3) :
    F = input('Voltage Test :')
    print('%.3E'%(toForce(F)))

plt.plot(F_list,inverseRf_list,'o',label='Original data',markersize=3)
plt.plot(np.asarray(F_list),Decimal(m)*np.asarray(F_list)+Decimal(c),'r',label='Fitted line')
plt.legend()
plt.show()





#include <SoftwareSerial.h>//串口通信库

//uno上的rx,tx 
SoftwareSerial pms5Serial(6, 7);

void setup() {
	//串口监视器初始化
	Serial.begin(115200);
	//pmg7传感器串口初始化
	pms5Serial.begin(9600);
	pinMode(7,OUTPUT);
}

//一个loop只读入一个字节的数据，总共40字节
//大气PM2.5是下标12 13
//甲醛 28 29（真实浓度mg/m3 = 该数据/1000）
//温度 30 31 摄氏度
//湿度 32 33 单位%
//温度&湿度的真实值都为 数据/10
//校验位 38 39 
void loop() {
	static unsigned char pms5_data[40];
	//记录读到第几个数据了
	static unsigned int data_num = 0;
	unsigned int pm2_5;
	if(pms5Serial.available()){
		pms5_data[data_num] = pms5Serial.read();
		data_num++;  
		//Serial.print(pmg7_data);
	}
	//读取完一次的数据包后，获取PM2.5值，复位
	if(data_num==40){
		//校验操作 还没测试过能不能用
		int confirm = 0;
		for(int i=0;i<38;i++){
			confirm+=pms5_data[i];
		}
		if(pms5_data[38]*256 + pms5_data[39] == confirm){
			pm2_5 = pmg7_data[12]*256 + pmg7_data[13]; 
			
			/* 其他操作 */
		
			Serial.println(pm2_5);
		}
		//复位
		data_num = 0;
	}
}

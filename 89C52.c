/*
**********************************************************************
说明:
摁下键盘1键显示日期，并且在此时摁下5，9调年份，6，0调节月，7,e调节日期
摁下键盘2键显示闹钟设置，5键设置小时，6键设置分钟
摁下键盘3键显示时间，5,6分别设置小时，分钟
摁下键盘4键第一次，显示生日，并依照设置时间的方式设置生日。
摁下B键，进入秒表及时，5键开始计时，6键停止计时，B键复位

Author:anx1ang  Date:2016-12-16
**********************************************************************
*/
#include<reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit dula=P2^6;
sbit wela=P2^7;
sbit led=P1^1;
sbit beep=P2^3;
uchar count=0;//count 100 time for 1s
uint weekday=1,date_count=0;
uchar j,k,temp,key=17,flag=0; 
//j,k for keyboard,key is order of keyboard,flag is select function
uint ahour=0,aminute=0;
//alarm colok varible
uint mms=0,msec=0,mmin=0;
//miaobiao varible
uchar mms_1,mms_0,msec_1,msec_0,mmin_1,mmin_0;
//toDisplayArgv varible
uint byear=1996,bmonth=10,bday=2,age,age_count=0;
//birthday varible
uchar byear_3,byear_2,byear_1,byear_0,bmonth_1,bmonth_0,bday_1,bday_0,age_1,age_0;
//Birthday toDisplayArg
uchar ahour_1,ahour_0,amin_1,amin_0;
//alarmToDisplayArgv
uchar sec_0=0,sec_1=0,min_0=0,min_1=0,hour_0=0,hour_1=0;
//timeToDisplay
uchar year_3=0,year_2=0,year_1=0,year_0=0,month_1=0,month_0=0,day_1=0,day_0=0;//DateToDisplay
uint second=30,minute=30,hour=12;//begin Time
uint year=2016, month=12,day=19;//begin Date
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//Table Display Code
void delay(uint time);
void LED_Display(uchar a,uchar b);
void argvToDisplay();
void timeCount();
void wholePoint();
void display(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f,uchar g,uchar h);
//delay for key_scan function
void delay(uint time)
{
	uchar i,j;
	for(i=time;i>0;i--)
		for(j=110;j>0;j--);
}
//function for key and select function and select function
void key_scan()
{
				    P3=0xfe;
				    temp=P3;
				    temp=temp&0xf0;
				    if(temp!=0xf0)
				    {
				      delay(10);
				      if(temp!=0xf0)
				      {
				        temp=P3;
				        switch(temp)
				        {
				          case 0xee:
				               key=2;flag=2;
//Display setting alarm
				               break;
				          case 0xde:
				               key=1;flag=1;date_count++;if(date_count==2){date_count=0;}
//Display setting date
				               break;
				          case 0xbe:
				               key=3;flag=0;
//for to show time
				               break;
				          case 0x7e:
				               key=4;flag=3;age_count++;if(age_count==2){age_count=0;}//to show birthday and age age_count=0 is birthday else is age
				               break;
				         }
				         while(temp!=0xf0)
				         {
				           temp=P3;
				           temp=temp&0xf0;
				           beep=0;
				         }
				         beep=1;
				      }
				    }
	    P3=0xfd;
	    temp=P3;
	    temp=temp&0xf0;
	    if(temp!=0xf0)
	    {
	      delay(10);
	      if(temp!=0xf0)
	      {
	        temp=P3;
	        switch(temp)
	        {
	          case 0xed:
	               key=6;
	               break;
	          case 0xdd:
	               key=5;
	               break;
	          case 0xbd:
	               key=7;  
	               break;
	          case 0x7d:
	               key=8;  
	               break;
	         }
	         while(temp!=0xf0)
	         {
	           temp=P3;
	           temp=temp&0xf0;
	           beep=0;
	         }
	         beep=1;
	      }
	      }
						    P3=0xfb;
						    temp=P3;
						    temp=temp&0xf0;
						    if(temp!=0xf0)
						    {
						      delay(10);
						      if(temp!=0xf0)
						      {
						        temp=P3;
						        switch(temp)
						        {
						          case 0xeb:
						               key=11;flag=4;
						               break;		
						          case 0xdb:
						               key=10; 
						               break;
						          case 0xbb:
						               key=12; 
						               break;
						          case 0x7b:
						               key=13;	
						               break;
						         }
						         while(temp!=0xf0)
						         {
						           temp=P3;
						           temp=temp&0xf0;
						           beep=0;
						         }
						         beep=1;
						      }
						      }
    P3=0xf7;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xe7:
               key=0;
               break;
          case 0xd7:
               key=9;
               break;
          case 0xb7:
               key=14;
               break;
          case 0x77:
               key=15;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
     }
  }
}
//Timer0 interrupt
void timer0() interrupt 1	         
{
   TH0=(65536-1*110592/12)/256;
	 TL0=(65536-1*110592/12)%256;
//10ms a count,count 100 is 1s
 	 count++;
	 if(key==6){
		mms=mms;
//if click 6 ,then mms stop, for to stop count mms
	 }else
	 {
		 mms++;
	 }
	 if(count==100)
 //count 100 is 1s
	 {	 
			 count=0;
	     second++;
	     timeCount();
	 }
}

//for to count time
void timeCount()
{
	wholePoint();
	if((ahour==hour)&&(aminute==minute)){
		if(second%2==0)
			beep=1;
		else
			beep=0;
	}
	argvToDisplay();
	if(second==60)
	{//60s -> 0s && minute+1
		second=0;
		minute++;
		if(minute==60)
		{//60m ->0m && hour+1
			minute=0;
			hour++;
			if(hour==24)
			{ //24h->0h && day+1
				hour=0;
				day++;
				if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
				{//if is big month ,have 31 day
					if(day==32)
					{
						day=1;
						month++;
						if(month==13) //13month->1month && year+1
						{
							month=1;
							year++;
						}
					}
				}
				else
				{
					//if is 2 month then judge is run nian?
					if(month==2)
					{
							if(((year%100!=0)&&(year%4==0))||(year%400==0))
							{ //is 30day->1day && mon+1
								if(day==30)
								{
									day=1;
									month++;								
								}
							}
							else
							{//no 29->1&&mon+1
								if(day==29)
								{
									day=1;
									month++;
								}
							}
					}
					else// 31->1 && month ++
					{ 
						if(day==31){
							day=1;
							month++;
						}
					}
				}
			}
		}
	}
}
//function for to argv to display
void argvToDisplay(){
	year_3=year/1000;
	year_2=year%1000/100;
	year_1=year%1000%100/10;
	year_0=year%1000%100%10;
	month_1=month/10;
	month_0=month%10;
	day_1=day/10;
	day_0=day%10;
	hour_1=hour/10;
	hour_0=hour%10;
	min_1=minute/10;
	min_0=minute%10;
	sec_1=second/10;
	sec_0=second%10;
	ahour_1=ahour/10;
	ahour_0=ahour%10;
	amin_1=aminute/10;
	amin_0=aminute%10;
	byear_3=byear/1000;
	byear_2=byear%1000/100;
	byear_1=byear%1000%100/10;
	byear_0=byear%1000%100%10;
	bmonth_1=bmonth/10;
	bmonth_0=bmonth%10;
	bday_1=bday/10;
	bday_0=bday%10;
	age_1=age/10;
	age_0=age%10;
	mms_1=mms/10;
	mms_0=mms%10;
	msec_1=msec/10;
	msec_0=msec%10;
	mmin_1=mmin/10;
	mmin_0=mmin%10;
}
//display for erjiguan
void display(uchar a,uchar b, uchar c,uchar d,uchar e,uchar f,uchar g,uchar h){
	LED_Display(a,1);		 
	LED_Display(b,2);		 
	LED_Display(c,3);		 
 	LED_Display(d,4);		
  LED_Display(e,5);		
	LED_Display(f,6);		 
 	LED_Display(g,7);		 
  LED_Display(h,8);
	led=~led;
	delay(1);
}
//argv look up table for display
void LED_Display(uchar number,uchar location )
{
//**********************  0    1    2    3    4    5    6    7    8    9    -    B    C    D    E    F    H *****//
uchar code LED_Segment[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0x7c,0x39,0x5e,0x79,0x71,0x76}; 
	//****************  OFF   1    2    3    4    5    6    7     8  *****/////
uchar code site[]={0xff,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};        
P0=LED_Segment[number];
dula=1;
dula=0;
P0=site[location];
wela=1; 
wela=0;
delay(1);
}
//for setting date
void setting_date(){
	if(key==5){
		year++; 
		key=17;
	}
	if(key==6){
		month++;
		key=17;
		if(month==13){month=1;}
	}
	if(key==7){
		day++;
		key=17;
		if(day==32){day=1;}
	}
	if(key==9){
		year--;
		key=17;
	}
	if(key==0){
		month--;
		key=17;
		if(month==0){month=12;}
	}
	if(key==14){
		day--;
		key=17;
		if(day==0){day=31;}
	}
}
//setting alarm
void setting_alarm(){
	if(key==5){
		ahour++;
		key=17;
		if(ahour==24){ahour=0;}
	}
	if(key==6){
		aminute++;
		key=17;
		if(aminute==60){aminute=0;}
	}
}
//setting time
void setting_weekday(){
	if(key==5){
		weekday++;
		if(weekday==8)
		{
			weekday=1;
			key=17;
		}
	}
}
void setting_time(){
	if(key==5){
		hour++;
		key=17;
		if(hour==24){hour=0;}
	}
	if(key==6){
		minute++;
		key=17;
		if(minute==60){minute=0;}
	}
}
//setting birth
void setting_birth(){
	if(key==5){
		byear++;
		key=17;
	}
	if(key==6){
		bmonth++;
		key=17;
		if(bmonth==13){bmonth=1;}
	}
	if(key==7){
		bday++;
		key=17;
		if(bday==32){bday=1;}
	}
	if(key==9){
		byear--;
		key=17;
	}
	if(key==0){
		bmonth--;
		key=17;
		if(bmonth==0){bmonth=12;}
	}
	if(key==14){
		bday--;
		key=17;
		if(bday==0){bday=31;}
	}
}
//miaobiao jishi
void runCountSec(){
	if(key==11)
	{
		mms=0;msec=0;mmin=0;
	}
	else if(key==5)
	{
	  if(mms==100)
		{
			mms=0;
			msec++;
			if(msec==60)
			{
				msec=0;
				mmin++;
				if(mmin==60)
				{
					mmin=0;
				}
			}
		}
	}
	else
	{
		mms=mms;
	}
}
void wholePoint(){
	if((minute==59)&&(second==59))
	{
		beep=0;
	}
	else if((minute==0)&&(second==0))
	{
		beep=1;
	}
}
//main function use timer0 
void main(){
	TMOD=0X01;//use timer0
	EA=1;//allow all interrupt
	ET0=1; 
	TR0=1;
	while(1)
	{
		timeCount();
		if(flag==1)
		{
			if(date_count==0)
			{
				display(year_3,year_2,year_1,year_0,month_1,month_0,day_1,day_0);
				setting_date();
			}
			else
			{
				display(month_1,month_0,day_1,day_0,10,10,10,weekday);
				setting_weekday();
			}
			key_scan();
		}
		else if(flag==2)
		{
			display(ahour_1,ahour_0,10,10,10,10,amin_1,amin_0);
			setting_alarm();
			key_scan();
		}
		else if(flag==3)
		{
			if(age_count==1)
			{
				display(byear_3,byear_2,byear_1,byear_0,bmonth_1,bmonth_0,bday_1,bday_0);
				setting_birth();
			}
			else
			{
				if(((year-byear)>0)&&((month-bmonth)>0)&&((day-bday)>0))
				{
					age=year-byear;
				}
				else
				{
					age=year-byear-1;
				}
				display(bmonth_1,bmonth_0,bday_1,bday_0,10,10,age_1,age_0);
			}
			key_scan();
		}
		else if(flag==4)
		{
			runCountSec();
			display(mmin_1,mmin_0,10,msec_1,msec_0,10,mms_1,mms_0);
			key_scan();
		}
		else
		{
			display(hour_1,hour_0,10,min_1,min_0,10,sec_1,sec_0);
			setting_time();
			key_scan();
		}
	}
}

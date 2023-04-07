#include "RTC_NXP.h"

RTC_NXP::RTC_NXP()
{
}

RTC_NXP::~RTC_NXP()
{
}

time_t RTC_NXP::time( time_t* tp )
{
	time_t	t	= rtc_time();
	if ( tp )
		*tp	= t;
	return t;
}

uint8_t	RTC_NXP::bcd2dec( uint8_t v )
{
	return (v >> 4) * 10 + (v & 0x0F);
}

uint8_t	RTC_NXP::dec2bcd( uint8_t v )
{
	return ((v / 10) << 4) + (v % 10);
}

PCF2131_I2C::PCF2131_I2C( uint8_t i2c_address ) : I2C_device( i2c_address )
{
}

PCF2131_I2C::~PCF2131_I2C()
{
}

time_t PCF2131_I2C::rtc_time()
{
	struct tm	now_tm;
	time_t		now_time;

	uint8_t		bf[ 8 ];
	
	r_seq( _100th_Seconds, bf, sizeof( bf ) );
	
	now_tm.tm_sec	= bcd2dec( bf[ 1 ] );
	now_tm.tm_min	= bcd2dec( bf[ 2 ] );
	now_tm.tm_hour	= bcd2dec( bf[ 3 ] );
	now_tm.tm_mday	= bcd2dec( bf[ 4 ] );
	now_tm.tm_mon	= bcd2dec( bf[ 6 ] ) - 1;
	now_tm.tm_year	= bcd2dec( bf[ 7 ] ) + 100;
	now_tm.tm_isdst	= 0;

	now_time	= mktime(&now_tm);
	
   return now_time;
}

int PCF2131_I2C::rtc_set( struct tm* now_tmp )
{
	time_t		now_time;
	struct tm*	cnv_tmp;

	uint8_t		bf[ 8 ];
	
	bf[ 0 ]	= 0;
	bf[ 1 ]	= dec2bcd( now_tmp->tm_sec  );
	bf[ 2 ]	= dec2bcd( now_tmp->tm_min  );
	bf[ 3 ]	= dec2bcd( now_tmp->tm_hour );
	bf[ 4 ]	= dec2bcd( now_tmp->tm_mday );
	bf[ 6 ]	= dec2bcd( now_tmp->tm_mon  ) + 1;
	bf[ 7 ]	= dec2bcd( now_tmp->tm_year ) - 100;

	now_time	= mktime( now_tmp );
	cnv_tmp		= localtime( &now_time );
	bf[ 5 ]		= dec2bcd( cnv_tmp->tm_wday);
	
	ow_reg( Control_1, ~0x28, 0x20 );
	ow_reg( SR_Reset,  (uint8_t)(~0x80), 0x80 );

	w_seq( _100th_Seconds, bf, sizeof( bf ) );

	ow_reg( Control_1, ~0x20, 0x00 );
	
	return 0;
}

bool PCF2131_I2C::oscillator_stop( void )
{
	return r_reg( Seconds ) & 0x80;
}

void PCF2131_I2C::w_seq( uint8_t reg, uint8_t *vp, int len )
{
	reg_w( reg, vp, len );
}

void PCF2131_I2C::r_seq( uint8_t reg, uint8_t *vp, int len )
{
	reg_r( reg, vp, len );
}

void PCF2131_I2C::w_reg( uint8_t reg, uint8_t val )
{
	reg_w( reg, val );
}

uint8_t PCF2131_I2C::r_reg( uint8_t reg )
{
	return 	reg_r( reg );
}

void PCF2131_I2C::ow_reg( uint8_t reg, uint8_t mask, uint8_t val )
{
	bit_op8( reg, mask, val );
}

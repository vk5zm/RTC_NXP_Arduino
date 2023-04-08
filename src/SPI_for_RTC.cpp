#include "RTC_NXP.h"

void SPI_for_RTC::txrx( uint8_t *data, uint16_t size )
{
	digitalWrite( SS, LOW );
	SPI.transfer( data, size );
	digitalWrite( SS, HIGH );
}

void SPI_for_RTC::reg_w( uint8_t reg_adr, uint8_t *data, uint16_t size )
{
	uint8_t	v[ size + 1 ];
	
	v[ 0 ]	= reg_adr;
	memcpy( v + 1, data, size );
	
	txrx( v, sizeof( v ) );
}

void SPI_for_RTC::reg_w( uint8_t reg_adr, uint8_t data )
{
	uint8_t	v[]	= { reg_adr, data };
	
	txrx( v, sizeof( v ) );
}

void SPI_for_RTC::reg_r( uint8_t reg_adr, uint8_t *data, uint16_t size )
{
	uint8_t	v[ size + 1 ] = { 0xFF };
	
	v[ 0 ]	= (uint8_t)(reg_adr | 0x80);
	
	txrx( v, sizeof( v ) );
	
	memcpy( data, v + 1, size );
}

uint8_t	SPI_for_RTC::reg_r( uint8_t reg_adr )
{
	uint8_t	v[]	= { (uint8_t)(reg_adr | 0x80), 0xFF };
	
	txrx( v, sizeof( v ) );
	
	return v[ 1 ];
}

void SPI_for_RTC::write_r8( uint8_t reg, uint8_t val )
{
	reg_w( reg, val );
}

uint8_t SPI_for_RTC::read_r8( uint8_t reg )
{
	return reg_r( reg );
}

void SPI_for_RTC::bit_op8(  uint8_t reg,  uint8_t mask,  uint8_t value )
{
	uint8_t	tmp	= reg_r( reg ) & mask;
	reg_w( reg, tmp | value );
}

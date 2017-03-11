
//
// gong_sensor
//
// send gong sound level to receiver
//
// author: @jefforulez
//

#include "pumpkinbot_cc.h"

//
// constants
//

const int LED_PIN_IDLE        = 3 ;
const int LED_PIN_PLUS_LEFT   = 5 ;
const int LED_PIN_PLUS_RIGHT  = 6 ;
const int LED_PIN_MINUS_LEFT  = 8 ;
const int LED_PIN_MINUS_RIGHT = 9 ;

const unsigned long REPORT_IP_INTERVAL = 30000 ;

const int BLINK_NUMBER = 10 ;
const int BLINK_DELAY  = 200 ;

//
// networking variables
//

// arduino mac address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE } ;

// @blip laptop
char SERVER_IP[] = "10.60.121.55" ;
const int SERVER_PORT = 46641 ;

// arduino server
EthernetServer server( 14664 ) ;

//
// global variables
//

unsigned long report_ip_last = 0 ;

//
// methods
//

void setup()
{
	//
	// initialize ethernet board
	//
	
	// configure ethernet board
	if ( Ethernet.begin( mac ) == 0 ) {
		while ( true ) ;
	}

	// wait for the board
	delay( 1000 ) ;
	
	//
	// confifure pins
	//
	
	pinMode( LED_PIN_IDLE, OUTPUT ) ;
	pinMode( LED_PIN_PLUS_LEFT, OUTPUT ) ;
	pinMode( LED_PIN_PLUS_RIGHT, OUTPUT ) ;
	pinMode( LED_PIN_MINUS_LEFT, OUTPUT ) ;
	pinMode( LED_PIN_MINUS_RIGHT, OUTPUT ) ;

	allOff() ;
}



void loop()
{ 	
	//
	// report IP to node server every REPORT_IP_INTERVAL milliseconds 
	//
	
	if ( 
		( millis() - report_ip_last > REPORT_IP_INTERVAL )
		|| ( report_ip_last == 0 )
	)
	{
		EthernetClient client_out ;

		// connect to the server and send level
		if ( client_out.connect( SERVER_IP, SERVER_PORT ) > 0 )
		{
			client_out.println( Ethernet.localIP() ) ;
			client_out.stop() ;
		}
		
		report_ip_last = millis() ;
		
		return ;
	}
	
	//
	// check for incoming connections
	//

	EthernetClient client = server.available() ;
	String command ;

	if ( client ) 
	{
		while ( client.connected() ) 
		{
			if ( client.available() ) 
			{
				char c = client.read() ;

				if ( ( c == '\n' ) || ( c == '\r' ) ) {
					break;
				}
				
				command += c ;
			}
		}
		client.stop() ;

		// echo the command back ( mostly for debugging )
		if ( command != "" )
		{
			// connect to the server and send level
			EthernetClient client_out ;
			if ( client_out.connect( SERVER_IP, SERVER_PORT ) > 0 )
			{
				client_out.print( command ) ;
				client_out.stop() ;
			}
		}

		if ( command == "plusplus" )
		{
			for ( int i = 0 ; i < BLINK_NUMBER ; i++ )
			{
				allOff() ;
				delay( BLINK_DELAY ) ;
				plusOn() ;
				delay( BLINK_DELAY ) ;
			}
		}
		else if ( command == "minusminus" )
		{
			for ( int i = 0 ; i < BLINK_NUMBER ; i++ )
			{
				allOff() ;
				delay( BLINK_DELAY ) ;
				minusOn() ;
				delay( BLINK_DELAY ) ;
			}
		}
		else if ( command == "blink" )
		{
			for ( int i = 0 ; i < BLINK_NUMBER ; i++ )
			{
				allOff() ;
				delay( BLINK_DELAY ) ;
				allOn() ;
				delay( BLINK_DELAY ) ;
			}
		}
		else
		{
			for ( int i = 0 ; i < BLINK_NUMBER / 2 ; i++ )
			{
				allOff() ;
				delay( BLINK_DELAY ) ;
				idleOn() ;
				delay( BLINK_DELAY ) ;
			}
		}

		allOff() ;
		delay( 1000 ) ;
		idleOn() ;
				
	}

	delay( 1000 ) ;
}

void allOn()
{
	digitalWrite( LED_PIN_IDLE, LOW ) ;
	digitalWrite( LED_PIN_PLUS_LEFT, LOW ) ;
	digitalWrite( LED_PIN_PLUS_RIGHT, LOW ) ;
	digitalWrite( LED_PIN_MINUS_LEFT, LOW ) ;
	digitalWrite( LED_PIN_MINUS_RIGHT, LOW ) ;
}

void allOff()
{
	digitalWrite( LED_PIN_IDLE, HIGH ) ;
	digitalWrite( LED_PIN_PLUS_LEFT, HIGH ) ;
	digitalWrite( LED_PIN_PLUS_RIGHT, HIGH ) ;
	digitalWrite( LED_PIN_MINUS_LEFT, HIGH ) ;
	digitalWrite( LED_PIN_MINUS_RIGHT, HIGH ) ;
}

void idleOn()
{
	digitalWrite( LED_PIN_IDLE, LOW ) ;
}

void plusOn()
{
	digitalWrite( LED_PIN_PLUS_LEFT, LOW ) ;
	digitalWrite( LED_PIN_PLUS_RIGHT, LOW ) ;
}

void minusOn()
{
	digitalWrite( LED_PIN_MINUS_LEFT, LOW ) ;
	digitalWrite( LED_PIN_MINUS_RIGHT, LOW ) ;
}


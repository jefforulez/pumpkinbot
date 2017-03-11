
var irc = require( 'irc' ) ;
var net = require( 'net' ) ;
var rl  = require( 'readline' ) ;

//
// constants
//

const IRC_SERVER = 'irc.corp.pokkari.net' ;
const IRC_CHANNEL = '#bliptv' ;

const IRC_HANDLE = 'pumpkinbot' ;
const IRC_REAL_NAME = 'pumpkinbot' ;
const IRC_DEBUG = true ;

const SERVER_PORT = 46641 ;

//
// arduino IP address
//

const ARDUINO_PORT = 14664 ;

var arduino_ip = "10.60.121.140" ;

//
// configure the irc client
//

var irc = new irc.Client(
	IRC_SERVER,
	IRC_HANDLE, 
	{
		channels : [ IRC_CHANNEL ],
		realName : IRC_REAL_NAME,
		//
		showErrors : true,
		debug : IRC_DEBUG,
	}
) ;

irc.addListener(
	'registered', 
	function ( message ) {
		console.log( "[registered] " ) ;
		console.log( message ) ;
	}
) ;

irc.addListener(
	'names', 
	function ( channel, nicks ) {
		console.log( "[names] channel : " + channel + ", nicks : " + nicks ) ;
	}
) ;


irc.addListener(
	'message', 
	function ( from, to, message ) 
	{
		console.log( "[message] from : " + from + ', to : ' + to + ', message : ' + message ) ;
		
		try
		{
		
		if ( from == "gongbot" )
		{
			if ( message.match( /GONG/ ) ) {
				sendCommandToArduino( 'blink' ) ;
			}
		}
		else if ( from == "plusplusbot" )
		{
			if (
				message.match( /^(ouch|daaaang|denied|ooooh|owie|awwww snap|ya dun goofed|boom|oh no you did not)/ )
			)
			{
				sendCommandToArduino( 'minusminus' ) ;
				irc.say( IRC_CHANNEL, "TRICK! HAHA!" ) ;
			}
			else if (
				message.match( /^(w00t|nice|suh-weet|well play|zing|you go girl|booyakasha|heyoooo|sweet|fist bump)/ )
			)
			{
				sendCommandToArduino( 'plusplus' ) ;
				irc.say( IRC_CHANNEL, "TREAT! YAY!" ) ;
			}
			else if (
				message.match( /^Sorry, you're trying to award too fast./ )
			)
			{
				sendCommandToArduino( 'blink' ) ;
				irc.say( IRC_CHANNEL, "Sorry?  There's no 'sorry' in " + IRC_CHANNEL ) ;
			}
			else
			{
				sendCommandToArduino( 'blink' ) ;
				irc.say( IRC_CHANNEL, "Trick or Treat?!?" ) ;
				console.log( '[learn] ' + message ) ;
			}
		}
		else if (
			( message.match( /pumpkinbot/ ) )
			&& ( ! message.match( /pumpkinbot\+\+/ ) )
			&& ( ! message.match( /pumpkinbot--/ ) )
		)
		{
			if ( message.match( /trick/ ) && ! message.match( /treat/ ) )
			{
				sendCommandToArduino( 'minusminus' ) ;
				irc.say( IRC_CHANNEL, "TRICK!" ) ;
			}
			else if ( ! message.match( /trick/ ) && message.match( /treat/ ) )
			{
				sendCommandToArduino( 'plusplus' ) ;
				irc.say( IRC_CHANNEL, "TREAT!" ) ;				
			}
			else
			{
				sendCommandToArduino( 'blink' ) ;
			
				var replies = new Array(
					"Happy Halloween!!",
					"Beware of Witches!",
					"R.I.P. gongbot",
					"Happy Haunting!",
					"BOOOO!!",
					"Spoooky!"
				) ;
			
				var i = Math.floor( ( Math.random() * ( replies.length - 1 ) ) ) ;
			
				irc.say( IRC_CHANNEL, replies[i] ) ;
			}
		}
		
		}
		catch ( e ) 
		{
			console.log( 'irc.message, e : ' + e ) ;
		}
		
	}
) ;


irc.addListener(
	'pm',
	function ( from, message ) 
	{
		console.log( "[pm] from : " + from + ', message : ' + message ) ;
		
		if ( from.match( /^jeffo/ ) )
		{
			sendCommandToArduino( message.trim() ) ;
		}
	}
) ;

irc.addListener(
	'error', 
	function ( message ) {
		console.log( "[error] " + message ) ;
	}
) ;


//
// send a command to the arduino
//

function sendCommandToArduino ( command )
{
	var client = net.connect(
		{ port : ARDUINO_PORT, host : arduino_ip },
		function() {
			console.log( 'client connected' ) ;
			client.write( command + '\r\n' ) ;
			console.log( 'client sent command: ' + command ) ;
			client.end() ;
		}
	) ;
	
	client.on( 
		'error',
		function ( m ) { console.log( 'client, m : ' + m ) ; }
	) ;

}

//
// configure the server
//

var server = net.createServer(
	function ( socket ) 
	{
		var i = rl.createInterface( socket, socket ) ;
		i.on( 
			'line', 
			function ( line ) { 
				console.log( "[server:line] " + line ) ;
				
				// set target ip address
				// arduino_ip = line.trim ;
			} 
		) ;
	}
) ;

server.listen(
	SERVER_PORT, 
	function () {
		console.log( '[server] bound' ) ;
	}
) ;

//
//

console.log( 'pumpkinbot.js started' ) ;

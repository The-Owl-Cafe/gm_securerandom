#include "GarrysMod/Lua/Interface.h"
#include <random>
#include <climits>
#include <vector>

std::random_device rd;

#if defined( _WIN32 ) && defined( _M_X64 )
std::mt19937_64 gen;
#else
std::mt19937 gen;
#endif

int randomNumber( lua_State * state ) {
	if ( LUA->GetType( 1 ) != GarrysMod::Lua::Type::NIL ) {	//Check if we have the first argument
		LUA->CheckType( 1, GarrysMod::Lua::Type::NUMBER );	//Verify the first argument is a number

		if ( LUA->GetType( 2 ) != GarrysMod::Lua::Type::NIL ) {	//Check if we have the second argument
			LUA->CheckType( 2, GarrysMod::Lua::Type::NUMBER );	//Verify the second argument is a number

			//Push our random number between the first and second argument
			std::uniform_real_distribution<double> dist( LUA->GetNumber( 1 ), LUA->GetNumber( 2 ) );
			LUA->PushNumber( dist( gen ) );
		} else {
			//Added in by The Owl Cafe
			//Purpose: To make it behave fully like the math.random function in glua, effectively allowing it to fully replace math.random

			//We only have the first argument
			//We return a number between 1 and the second argument
			std::uniform_real_distribution<double> dist( 1.0, LUA->GetNumber( 1 ) );
			LUA->PushNumber( dist( gen ) );
		}
	} else {
		//Return a random number between 0 and 1
		std::uniform_real_distribution<double> dist;
		LUA->PushNumber( dist( gen ) );
	}

	return 1;
}

int randomWholeNumber( lua_State * state ) {
	if ( LUA->GetType( 1 ) != GarrysMod::Lua::Type::NIL ) {	//Check if we have the first argument
		LUA->CheckType( 1, GarrysMod::Lua::Type::NUMBER );	//Verify the first argument is a number

		if ( LUA->GetType( 2 ) != GarrysMod::Lua::Type::NIL ) {	//Check if we have the second argument
			LUA->CheckType( 2, GarrysMod::Lua::Type::NUMBER );	//Verify the second argument is a number

			//Push our random number between the first and second argument
			std::uniform_int_distribution<int> dist( LUA->GetNumber( 1 ), LUA->GetNumber( 2 ) );
			LUA->PushNumber( dist( gen ) );
		} else {
			//Added in by The Owl Cafe
			//Purpose: To make it behave fully like the math.random function in glua, effectively allowing it to fully replace math.random

			//We only have the first argument
			//We return a number between 1 and the second argument
			std::uniform_int_distribution<int> dist( 1, LUA->GetNumber( 1 ) );
			LUA->PushNumber( dist( gen ) );
		}
	} else {
		//Return a random number between 0 and 1
		std::uniform_int_distribution<int> dist;
		LUA->PushNumber( dist( gen ) );
	}

	return 1;
}

int randomBytes( lua_State * state ) {
	LUA->CheckType( 1, GarrysMod::Lua::Type::NUMBER );
	const double requestedSize = LUA->GetNumber( 1 );
	const size_t size = floor( LUA->GetNumber( 1 ) );

	if ( requestedSize < 0 || size != requestedSize )
		LUA->ThrowError( "size must be a whole number >= 0" );

	std::uniform_int_distribution<int> dist( SCHAR_MIN, SCHAR_MAX );
	std::vector<char> bytes;
	bytes.resize( size );

	for ( size_t i = 0; i < size; i++ )
		bytes[ i ] = ( char ) dist( rd );

	LUA->PushString( bytes.data(), size );

	return 1;
}

int randomSeed( lua_State * state ) {
	if ( LUA->GetType( 1 ) != GarrysMod::Lua::Type::NIL ) {//Check if we supplied an argument
		LUA->CheckType( 1, GarrysMod::Lua::Type::NUMBER );//Check if our argument is a number

		//Reset the seed first, otherwise we end up with seeds that are not consistant
		gen.seed( rd() );
		gen.discard( 700000 );

		//Set our new seed to the number we gave it from lua
		gen.seed( LUA->GetNumber( 1 ) );
	} else {
		gen.seed( rd() );//If we did not pass an argument then reset the seed instead
		gen.discard( 700000 );
	}

	return 0;
}

GMOD_MODULE_OPEN() {
	gen.seed( rd() );
	gen.discard( 700000 ); // http://www.iro.umontreal.ca/~lecuyer/myftp/papers/lfsr04.pdf page 11

	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
	LUA->CreateTable();

	//Allow us to define our own seeds if we need to
	LUA->PushCFunction( randomSeed );
	LUA->SetField( -2, "RandomSeed" );

	LUA->PushCFunction( randomNumber );
	LUA->SetField( -2, "Number" );

	//Same as random.Number, but should provide a whole number instead of a decimal
	LUA->PushCFunction( randomWholeNumber );
	LUA->SetField( -2, "WholeNumber" );

	LUA->PushCFunction( randomBytes );
	LUA->SetField( -2, "Bytes" );

	LUA->SetField( -2, "random" );
	LUA->Pop();
	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}

# gm_securerandom
A module for generating secure random numbers and strings in Garry's Mod

## API

#### random.Bytes(size)

* size (number) - A whole number representing the amount of bytes you want returned

Generates `size` bytes and returns them as a string.

#### random.Number([min, max])

* min (number, optional, default 0) - The minimum value to generate
* max (number, optional, default 1) - The maximum value to generate

Generates a number between `min` and `max` and returns it.

## Usage

#### Random bytes
```lua
require( "securerandom" )
print( util.Base64Encode( random.Bytes( 16 ) ) )
```

#### Random number between 0 and 1
```lua
require( "securerandom" )
print( random.Number() )
```

#### Random number between 1 and 20.5
```lua
require( "securerandom" )
print( random.Number( 20.5 ) )
```

#### Random number between 5 and 10.5
```lua
require( "securerandom" )
print( random.Number( 5, 10.5 ) )
```


#### Random number between 0 and 1
```lua
require( "securerandom" )
print( random.WholeNumber() )
```

#### Random whole number between 1 and 20
```lua
require( "securerandom" )
print( random.WholeNumber( 20 ) )
```

#### Random whole number between 5 and 10
```lua
require( "securerandom" )
print( random.Number( 5, 10 ) )
```


#### Changes the seed to 1234 and prints the same number 5 and 10 in the console multiple times
```lua
require( "securerandom" )
for i = 1, 10 do
	random.RandomSeed( 1234 )
	print( random.Number( 5, 10 ) )
end
```

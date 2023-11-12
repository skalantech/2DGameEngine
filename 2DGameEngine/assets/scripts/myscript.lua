-- This a Lua global variable
some_variable = 2^(5 * 2)
user_name = "Carlos Escalante"

print("Hello " ..user_name)

if some_variable == 42 then
	print("The value of the variable is "..some_variable)
end

-- This a lua table that contains values for the initialization of our game engine
config = {
	title = "My Game Engine",
	fullscren = false,
	resolution = {
		width = 800, 
		height = 600
	}
}

-- This is a Lua function that calculates the factorial of a given number
function factorial1(n)
	if n == 0 then 
		return 1
	end 
		return n * factorial1(n - 1)
end

function factorial(n)
	i = n
	result = 1
	while i > 1 do
		result = result * i
		i = i - 1
	end
	print(result)
end

function factorial2(n)
	sum = 1
	for i = n, 1, -1 do
		sum = sum * i
	end
	print (sum)
end

factorial(5)
factorial(7)
factorial(3)
print("Factorial of 5 is " ..factorial1(5))

print("The cube of the number 3 is equal to ".. cube(3))
factorial2(10)

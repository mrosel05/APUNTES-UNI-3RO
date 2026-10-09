import System.Win32 (xBUTTON1)
import GHC.Internal.Text.Read.Lex (Number)
import Data.Char (toUpper)
--Ej 1
areaCirculo r = r*r*pi

--Ej 2
esBisiesto n 
    | mod n 400 == 0 = True
    | mod n 100 == 0 = False
    | mod n 4 == 0 = True
    | otherwise = True

--Ej 3
celsiusAFahrenheit :: Float -> Float
celsiusAFahrenheit c = c * 9 / 5 + 32

--Ej 4
maxDeTres x y z = 
    if x >= y then 
        if x >= z then x
        else z
    else if y >= z then y
        else z

--Ej 5
rC a b c = ((negate b + raiz)/(2.0*a), (negate b - raiz)/(2.0*a))
    where raiz = sqrt(b*b - 4*a*c)

--Ej 6
clasificaIMC peso altura
    | imc < 18.5 = "bajo peso"
    | imc < 25 = "normal"
    | imc < 30 = "sobrepeso"
    | otherwise = "obesidad"
        where imc = peso / (altura * altura)

--Ej 7
primero3 xs = take 3 xs

--Ej 8 
ultimoElemento xs = last xs

--Ej 9
intercambiarPar (a,b) = (b,a)

--Ej 10
aplicarDosVeces f x = f(f(x))

--Ej 11
contarOcurrencias a xs = length(filter (==a) xs)

--Ej 12 
eliminaTodas a xs = filter (/= a) xs

--Ej 13
cuadradosPares xs = map (^2) (filter even xs)

--Ej 14
tablaMultiplicar n = map (*n) [1..10]

--Ej 15
palabraEnMayusculas s = map toUpper s

--Ej 16 
sumaPositivos xs = sum(filter (>0) xs)

--Ej 17
--a) p1 -> p2 -> p1
--b) p1 -> p2 -> (p1, p2)
--c) (t -> t) -> t -> t
--d) [(b1, b2)] -> [b1]
--e) (Eq a, Num a) => [a] -> [a]
--f) (Num a, Enum a) => [b] -> [(a, b)]

--Ej 18
--a) Number distinto de bool 
--b) suma recibe tupla, no dos int
--c) 1 no es booleano
--d) factorial requiere int
--e) lista vacia

--Ej 19 
f1 x = x == x

f2 x y = if x then y else y + 1

--f3 xs = head xs : xs

f4 g x = g (g x)

--Ej 20
--a) Num a => [a] -> [a]
--b) (Ord a, Num a) => [a] -> [a]
--c) Integral a => [[a]] -> [[a]]
import Data.Char
import Data.List

--Ej 1
--multiplosDe :: Int -> Int -> [Int]
--multiplosDe n l = map (*n) [1..l `div` n]
--multiplosDe n l = filter (\x -> mod x n == 0) [1..l]
multiplosDe n l = [x | x<-[1..l], mod x n == 0]

--Ej 2
--sumaCuadradosImpares n = sum $ map (^2) $ filter (\x -> mod x 2 == 1) [1..n]
sumaCuadradosImpares n = sum [x^2 | x<-[1..n] , mod x 2 == 1]

--Ej 3
triangulosRectangulos n = [(a, b, c) | c <- [1..n], b <- [1..c], a <- [1..b]
    , a^2 + b^2 == c^2, mod (a+b+c) 2 == 0]

--Ej 4
productoCartesiano xs ys = [(a, b) | a<-xs, b <- ys]

--Ej 5
posicionEnAlfabeto a = ord a - ord 'a'

--Ej 6
letraEnPosicion x = chr ((mod x 26) + ord 'a')

--Ej 7
desplazaLetra x a 
    | isLower a = letraEnPosicion $ (posicionEnAlfabeto a + x)
    | otherwise = a

--Ej 8
cifradoCesar x xs = [desplazaLetra x a | a <- xs]

--Ej 9 
descifradoCesar x xs = cifradoCesar (-x) xs

--Ej 10
count a [] = 0
count a (x:xs)
    | a == x = 1 + count a xs
    | otherwise = count a xs

sinRepes [] = []
sinRepes (x:xs) = x : sinRepes (filter (/= x) xs)


--no salen en orden
--frecuenciaLetras xs = [(x, count x xs)| x <- ['a'..'z'], count x xs > 0 ]
frecuenciaLetras xs = [(x, count x xs)| x<-nub xs, isLower x, count x xs > 0]

--Ej 11
descifradoFuerzaBruta xs = [descifradoCesar x xs| x <- [0..25]]

--Ej 12
repiteClave xs x = take x $ cycle xs

--Ej 13 
calcularPosiciones xs num = [posicionEnAlfabeto x | x<-repiteClave xs num]

cifrar _ [] = []
cifrar (x:xs) (y:ys) 
    | isLower y = (desplazaLetra x y) : cifrar xs ys
    | otherwise = y : cifrar (x:xs) ys

cifradoVigenere cla men = 
    cifrar despla men
    where 
        despla = calcularPosiciones cla $ length men

--Ej 14
palabraOpuesta xs = [letraEnPosicion (26 - posicionEnAlfabeto x) | x<-xs]

descifradoVigenere cla men = cifradoVigenere cla2 men
    where cla2 = palabraOpuesta cla
        

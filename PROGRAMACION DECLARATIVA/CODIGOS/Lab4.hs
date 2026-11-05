import Data.Char   
import Data.Maybe

--Ej 1
miMap b [] = []
miMap b (x:xs) = (b x) : miMap b xs

--Ej 2
miFilter b [] = []
miFilter b (x:xs)
    | b x = x: miFilter b xs
    | otherwise = miFilter b xs

--Ej 3
miZipWith op [] [] = []
miZipWith op [] [a] = [a]
miZipWith op [a] [] = [a]
miZipWith op (x:xs) (y:ys) = op x y : miZipWith op xs ys

--Ej 4 
miAny b [] = False
miAny b (x:xs) = b x || miAny b xs

miAll b [] = True 
miAll b (x:xs) = b x && miAll b xs

--Ej 5
miMapFoldr b xs = foldr (\x acc -> b x: acc) [] xs 

--Ej 6 
miFilterFoldr b xs = foldr (\x acc -> if b x then x:acc else acc) [] xs

--Ej 7
miConcatMap b xs = foldr (\x acc -> (b x) ++ acc) [] xs

--Ej 8
triplicaPositivos = map (*3) . filter (>0)

--Ej 9
contarQueCumplen b = length . filter b

--Ej 10
tuberia op x = foldl (\acc o -> o acc) x op

--Ej 11
mayorPor p xs ys = (p xs) > (p ys)

--Ej 12 
validador [] _ = True 
validador (o:op) x = o x && (validador op x)

--Ej 13
pn acc [] = acc
pn acc (x:xs) 
    | x >= 5 = (sum + x, num + 1, ap + 1, sus)
    |otherwise = (sum + x, num + 1, ap, sus + 1)
    where 
        (sum, num, ap, sus) = pn acc xs

procesarNotas xs = (sum/num, ap, sus)
    where (sum, num, ap, sus) = pn (0,0,0,0) xs 

--Ej 14 
capPalabra (x:xs) = chr(ord x - 32): xs

palabrasCapitalizadas xs = map (\x -> capPalabra x) (words xs)

--Ej 15
--reglas = [("edad", \v-> length v > 0 && all (`elem` ['0'..'9']) v),("nombre", \v-> length v > 0)]
--registro = [("edad", "abc"), ("nombre", "Ana"), ("extra", "error")]
camposInvalidos :: [(String, String-> Bool)]-> [(String, String)]-> [String]
camposInvalidos [] []  = []
camposInvalidos [] ((r,nombre):regis) = nombre: camposInvalidos [] regis
camposInvalidos ((campo, compro):rs) (r:regis) 
    | compro valor =  camposInvalidos rs regis
    | otherwise = valor: camposInvalidos rs regis
    where Just valor = lookup campo regis




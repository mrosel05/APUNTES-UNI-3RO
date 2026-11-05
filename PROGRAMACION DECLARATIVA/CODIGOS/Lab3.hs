


--Ej 1
miUnzip2 acc [] = acc
miUnzip2 (ac1, ac2) ((a,b):xs) = miUnzip2 (ac1 ++ [a], ac2 ++ [b]) xs

miUnzip xs = miUnzip2 ([], []) xs

--Ej 2 
contarVocales2 acc [] = acc
contarVocales2 acc (x:xs) 
    | esVocal x = contarVocales2 (acc+1) xs
    | otherwise = contarVocales2 acc xs
    where 
        esVocal x = x == 'a' || x == 'e' || x == 'i' || x == 'o' || x == 'u'

contarVocales xs = contarVocales2 0 xs

--Ej 3
ua (a1, a2) _ [] = a1
ua (a1, a2) valor (x:xs)
    | valor == x = ua (a2, a2+1) valor xs 
    | otherwise = ua (a1, a2+1) valor xs

ultimaAparicion x xs = ua (-1, 0) x xs

--Ej 4 
intercalar [] ys = ys
intercalar xs [] = xs
intercalar (x:xs) (y:ys) = x:y:intercalar xs ys

--Ej 5 
particionar piv [] = ([],[],[])
particionar piv (x:xs) 
    | x < piv = (x:x1, x2, x3)
    | x == piv = (x1, x:x2, x3)
    | x > piv = (x1, x2, x:x3)
    where  
        (x1, x2, x3) = particionar piv xs

--Ej 6
particionarPredicado b [] = ([],[])
particionarPredicado b (x:xs) 
    | b x = (x:x1,x2)
    | otherwise = (x1,x:x2)
    where   
        (x1,x2) = particionarPredicado b xs 
 

--Ej 7
insertarDescendente num [] = [num]
insertarDescendente num (x:xs)
    | x <= num = num:x:xs
    | otherwise = x: insertarDescendente num xs


--Ej 8
estaOrdenada [] = True
estaOrdenada [a] = True
estaOrdenada (x:y:xs) 
    | x <= y = estaOrdenada xs
    | otherwise = False

--Ej 9
numeroInversiones [] = 0
numeroInversiones (x:xs) = length (filter (<x) xs) + numeroInversiones xs

--Ej 10
miAcumulado acc [] = acc
miAcumulado acc (x:xs) = miAcumulado (acc + x) xs

sumaAcc xs = miAcumulado 0 xs

--Ej 11
miAcumulado2 acc [] = acc
miAcumulado2 acc (x:xs) 
    | acc >= x = miAcumulado2 acc xs
    | otherwise = miAcumulado2 x xs

maximoAcc (x:xs) = miAcumulado2 x xs

--Ej 12 
runLength [] = []
runLength (x:xs) 
    = (x, 1 + length xs - length lista):runLength lista
    where 
        lista = dropWhile (==x) xs

--Ej 13
dividir [] = ([],[])
dividir [a] = ([a], [])
dividir (x:y:xs) = (x:a, y:b)
    where (a, b) = dividir xs

fusionar [] xs = xs
fusionar xs [] = xs
fusionar (x:xs) (y:ys) 
    | x <= y = x: fusionar xs (y:ys)
    | otherwise = y: fusionar (x:xs) ys

mergesort [] = []
mergesort [a] = [a]
mergesort xs = fusionar (mergesort a) (mergesort b)
    where (a, b) = dividir xs
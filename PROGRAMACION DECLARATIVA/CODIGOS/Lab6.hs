import Data.Map (valid)
import System.Win32 (xBUTTON1)
import Text.XHtml (ins)
--Ej 1
data Pila a = CPila [a]
    deriving (Eq, Show)

push :: a-> Pila a-> Pila a
push a (CPila xs) = (CPila (a:xs))

pop :: Pila a -> Maybe (a, Pila a)
pop pila = 
    case pila of 
        CPila [] -> Nothing
        CPila (x:xs) -> Just (x, CPila xs)

estaVacia :: Pila a-> Bool
estaVacia (CPila []) = True
estaVacia (CPila xs) = False


--2
data Cola a = CCola [a]
    deriving (Eq, Show)

encolar :: a-> Cola a-> Cola a
encolar a (CCola xs) =  (CCola (xs ++ [a]))

desencolar:: Cola a-> Maybe (a, Cola a)
desencolar (CCola []) = Nothing
desencolar (CCola (x:xs)) = Just (x, CCola xs)

--3 
data ArbolN a = NodoN a [ArbolN a]
    deriving (Show)

arbolN = NodoN 1 [NodoN 2 [], NodoN 3 [NodoN 4 []]]

tamanoN :: ArbolN a-> Int
-- tamanoN arbol =
--     case arbol of 
--         (NodoN _ []) -> 1
--         (NodoN a (x:xs)) -> (tamanoN x) + (tamanoN (NodoN a xs))
tamanoN (NodoN _ []) = 1
tamanoN (NodoN a (x:xs)) = (tamanoN x) + (tamanoN (NodoN a xs))

sumaN :: Num a => ArbolN a-> a
sumaN (NodoN a []) = a
sumaN (NodoN a (x:xs)) = a + (sumaN x) + (sumaN (NodoN 0 xs))

--4 
profundidadN :: ArbolN a -> Int
profundidadN (NodoN _ []) = 1
profundidadN (NodoN a (x:xs)) = 1 + (max (profundidadN x) ((profundidadN (NodoN a xs)) - 1))

--5
mapArbolAux :: (a -> b) -> [ArbolN a] -> [ArbolN b]
mapArbolAux _ [] = []
mapArbolAux f (x:xs) = mapArbolN f x : mapArbolAux f xs

mapArbolN :: (a-> b)-> ArbolN a-> ArbolN b
mapArbolN f (NodoN a []) = (NodoN (f a) [])
mapArbolN f (NodoN a xs) = (NodoN (f a) (mapArbolAux f xs))

--6 
data Producto = CProducto String Double Int 
    deriving Show

inventario = [CProducto "Teclado" 25 10, CProducto "Raton" 15 20]

buscarProducto :: String-> [Producto]-> Maybe Producto
buscarProducto _ [] = Nothing
buscarProducto s ((CProducto str d i):xs) 
    | s == str = Just (CProducto str d i)
    | otherwise = buscarProducto s xs

--7
dividirSeguro :: Int-> Int-> Either String Int
dividirSeguro n1 n2 
    | n2 == 0 = Left "No se puede dividir entre 0"
    | otherwise = Right (div n1 n2)

--8 
validarEdad :: Int-> Either String Int
validarEdad e 
    | e > 120 = Left "Edad fuera de rango razonable"
    | e < 0 = Left "La edad no puede ser negativa"
    | otherwise = Right e

--9
combinarEither :: Either String a-> Either String b-> Either String (a,b)
combinarEither e1 e2 = 
    case e1 of
        Left mensaje -> Left mensaje
        Right a -> 
            case e2 of 
                Left mensaje2 -> Left mensaje2
                Right b -> Right (a,b)

--10
data Consulta = Precio Double | NoEncontrado | SinStock
    deriving Show

calcularPrecioFinal :: String -> Double -> [Producto] -> Either String Double
calcularPrecioFinal s n inventario = 
    case buscarProducto s inventario of 
        Nothing -> Left "Producto no encontrado"
        Just (CProducto str prec stock)  
            | stock == 0 -> Left "Sin stock disponible"
            | otherwise -> Right (prec * (1 - n / 100))

--11 
data Triple a b c = CTriple a b c
    deriving Show

primero (CTriple a _ _) = a
segundo (CTriple _ b _) = b
tercero (CTriple _ _ c) = c

mapTriple f1 f2 f3 t = (CTriple (f1 $ primero t) (f2 $ segundo t) (f3 $ tercero t))


--12
zipTriple :: [a] -> [b] -> [c] -> [Triple a b c]
zipTriple _ _ [] = []
zipTriple _ [] _ = []
zipTriple [] _ _ = []
zipTriple (x:xs) (y:ys) (z:zs) = (CTriple x y z):(zipTriple xs ys zs)

--13
data ArbolBin a = HojaBin | NodoBin (ArbolBin a) a (ArbolBin a)
    deriving Show

insertarBT :: Ord a => a -> ArbolBin a -> ArbolBin a
insertarBT x HojaBin = (NodoBin HojaBin x HojaBin)
insertarBT x (NodoBin izq a der)
    | x < a = (NodoBin (insertarBT x izq) a der)
    | otherwise = (NodoBin izq a (insertarBT x der))

arbol = insertarBT 5 (insertarBT 3 (insertarBT 8 HojaBin))

perteneceBT x HojaBin = False
perteneceBT x (NodoBin izq a der)
    | x < a = perteneceBT x izq
    | x > a = perteneceBT x der
    | otherwise = True

--14 
listaAArbolAux :: Ord a => ArbolBin a -> [a] -> ArbolBin a
listaAArbolAux acum [] = acum
listaAArbolAux accum (x:xs) = listaAArbolAux acumulado xs
    where acumulado = insertarBT x accum

listaAArbol :: Ord a => [a]-> ArbolBin a
listaAArbol xs = listaAArbolAux HojaBin xs


--15 
data Expresion a = Lit a
    | Suma (Expresion a) (Expresion a)
    | SiMayor   (Expresion a) (Expresion a)
                (Expresion a) (Expresion a)
    deriving Show

evaluarExpresion :: (Ord a, Num a) => Expresion a-> a
evaluarExpresion (Lit a) = a
evaluarExpresion (Suma ex1 ex2) = (evaluarExpresion ex1) + (evaluarExpresion ex2)
evaluarExpresion (SiMayor e1 e2 e3 e4)
    | (evaluarExpresion e1) > (evaluarExpresion e2) = evaluarExpresion e3
    | otherwise = evaluarExpresion e4

ejemplo = SiMayor (Lit 5) (Lit 3) (Suma (Suma (Lit 2) (Lit 3)) (Lit 1)) (Lit 0)
    


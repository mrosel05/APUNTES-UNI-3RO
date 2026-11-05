--1
data Fecha = CFecha Int Int Int
    deriving (Show)

esAnterior (CFecha d1 m1 a1) (CFecha d2 m2 a2) 
    | a1 < a2 = True
    | m1 < m2 = True
    | d1 < d2 = True
    | otherwise = False

--2 
data Vector2D = CVector2D Double Double
    deriving (Show)

sumaVectores (CVector2D x1 y1) (CVector2D x2 y2) = CVector2D (x1+x2) (y1+y2)

escala num (CVector2D x1 y1) = CVector2D (x1*num) (y1*num)

modulo (CVector2D x y) = sqrt (x^2 + y^2)


--3 
data Libro = CLibro String Int Bool
    deriving (Show)

disponible (CLibro _ _ b) = not b

prestar (CLibro s i b) = CLibro s i (not b)


--4 
data Moneda = Euro | Dolar | Libra 
    deriving (Eq, Show)

tasaCambio Euro = 1
tasaCambio Dolar = 0.8
tasaCambio Libra = 1.25

convertir m1 m2 cant = cant * (tasaCambio m1) / (tasaCambio m2)

--5
data Palo = Picas | Corazones | Diamantes | Treboles 
    deriving (Eq, Show)
data Carta = CCarta Int Palo 
    deriving (Eq, Show)

instance Ord Carta where 
    compare (CCarta i1 _) (CCarta i2 _) = compare i1 i2

--6
buscarPorId idBusc lista = lookup idBusc lista

--7
combinarVectores Nothing _ = Nothing
combinarVectores _ Nothing = Nothing
combinarVectores (Just v1) (Just v2) = Just (sumaVectores v1 v2)

--8 
data Entrada = Archivo String Int | Carpeta String [Entrada]

tamanoTotal :: Entrada -> Int
tamanoTotal (Archivo _ i) = i
tamanoTotal (Carpeta _ xs) = tamanoTotalLista xs

tamanoTotalLista [] = 0
tamanoTotalLista (x:xs) = (tamanoTotal x) + (tamanoTotalLista xs)

--9
contarArchivos (Archivo _ _ ) = 1
contarArchivos (Carpeta _ xs) = contarArchivosLista xs

contarArchivosLista [] = 0
contarArchivosLista (x:xs) = contarArchivos x + contarArchivosLista xs

--10
buscarArchivo s (Archivo nombre tam) 
    | s == nombre = Just tam
    | otherwise = Nothing

buscarArchivo s (Carpeta _ xs) = buscarArchivoLista s xs

buscarArchivoLista _ [] = Nothing
buscarArchivoLista s (x:xs) 
    | busqueda == Nothing = buscarArchivoLista s xs
    | otherwise = busqueda
    where 
        busqueda = buscarArchivo s x

--11
data BoolExpr = BVal Bool | BAnd BoolExpr BoolExpr| BOr BoolExpr BoolExpr | BNot BoolExpr
    deriving (Show)

evalB (BVal b) = b
evalB (BAnd b1 b2) = (evalB b1) && (evalB b2)
evalB (BOr b1 b2) = (evalB b1) || (evalB b2)
evalB (BNot b) = not (evalB b)

--12
simplificar (BVal b) = (BVal b)
simplificar (BAnd (BVal False) _) = BVal False
simplificar (BAnd _ (BVal False)) = BVal False
simplificar (BOr (BVal True) _) = BVal True
simplificar (BOr _ (BVal True)) = BVal True
simplificar (BAnd b1 b2) = (BAnd (simplificar b1) (simplificar b2))
simplificar (BOr b1 b2) = (BOr (simplificar b1) (simplificar b2))
simplificar (BNot b) = (BNot (simplificar b))

--13
data Producto = CProducto String Double Int
    deriving (Show, Eq)

valorTotalInventario = sum . map (\(CProducto _ precio stock) -> fromIntegral stock * precio)


--14 
buscarProducto _ [] = Nothing
buscarProducto nombre ((CProducto s d i):xs)
    | nombre == s = Just (CProducto s d i)
    | otherwise = buscarProducto nombre xs

aplicarDescuento desc (CProducto s d i) = (CProducto s (d - d * desc / 100) i ) 

--15º
inventario = [CProducto "Teclado" 25 10, CProducto "Raton" 15 20]
calcularPrecioFinal nombre desc invent 
    | buscado == Nothing = Left "Producto no encontrado"
    | otherwise = Right (aplicarDescuentoAux desc buscado)
    where buscado = buscarProducto nombre invent

aplicarDescuentoAux desc (Just prod) = precioFinal
    where (CProducto _ precioFinal _) = aplicarDescuento desc prod

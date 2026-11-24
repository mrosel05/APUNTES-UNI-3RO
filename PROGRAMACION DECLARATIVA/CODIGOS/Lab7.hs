import Distribution.Compat.Lens (_1)
import Distribution.Compat.Prelude (exitSuccess)
--1
data Prioridad = Baja | Media | Alta
    deriving (Show, Eq, Ord)

data Tarea = CTarea String Prioridad Bool
    deriving (Show, Eq)

instance Ord Tarea where 
    compare (CTarea _ p1 _) (CTarea _ p2 _) = compare p1 p2

descripcion (CTarea s _ _) = s
prioridad (CTarea _ p _) = p
completada (CTarea _ _ b) = b


--2 
añadirTarea s p xs = (CTarea s p False):xs

tareas = añadirTarea "Estudiar Haskell" Alta []

--3 
completar s [] = []
completar s ((CTarea desc p b):xs) 
    | s == desc = (CTarea desc p True):xs
    | otherwise = (CTarea desc p b):(completar s xs)

t2 = añadirTarea "Leer libro" Media tareas
completadas = completar "Estudiar Haskell" t2

--4 
pendientes [] = []
pendientes ((CTarea s p b):xs) 
    | b == False = (CTarea s p b):(pendientes xs)
    | otherwise = pendientes xs


porPrioridad [] = []
porPrioridad xs = quicksortPrioridad xs []

quicksortPrioridad [] acc = acc
quicksortPrioridad (x:xs) acc = 
    quicksortPrioridad iz (x: quicksortPrioridad der acc)
    where (iz, der) = particion [] [] x xs

particion iz der _ [] = (iz, der)
particion iz der p (x:xs) 
    | x >= p = particion (x:iz) der p xs
    | otherwise = particion iz (x:der) p xs

--5
buscarTarea _ [] = Nothing
buscarTarea s (x:xs) 
    | (descripcion x) == s = Just x
    | otherwise = buscarTarea s xs

--6
contarPendientes xs = length (pendientes xs)

esUrgente x = prioridad x == Alta && completada x == False

hayTareaUrgente [] = False
hayTareaUrgente (x:xs) 
    | prioridad x == Alta && completada x == False = True
    | otherwise = hayTareaUrgente xs

--7
mostrarTarea t = mostrarCompletado (completada t) ++ mostrarPrioridad (prioridad t) ++ descripcion t

mostrarCompletado b 
    | b = "[x] "
    | otherwise = "[ ] "

mostrarPrioridad p =
    case p of 
        Alta -> "[ALTA] "
        Media -> "[MEDIA] "
        Baja -> "[BAJA] "

bucle:: [Tarea] -> IO ()
bucle tareas = do
    putStr (unlines
            ["Anadir tarea (A)", "Marcar como completada (C)", 
            "Mostrar pendientes por prioridad (P)", "Mostrar todas (M)",
            "Solo tareas urgentes (U)", "Salir (S)", "Elige una opcion:"])
    opt <- getLine
    case opt of
        "A" -> do
            putStr "Introduce la descripcion: "
            desc <- getLine
            putStr "Introduce la prioridad (A, M, B): "
            prioStr <- getLine
            let prio = leerPrioridad prioStr
                nuevasTareas = añadirTarea desc prio tareas
            bucle nuevasTareas

        "C" -> do 
            putStr "Introduce la descripcion: "
            desc <- getLine
            let nuevasTareas = completar desc tareas
            bucle nuevasTareas

        "P" -> do
            putStr "Tareas no completadas ordenadas por prioridad: \n"
            let nuevasTareas = porPrioridad (pendientes tareas)
            putStr (unlines (map mostrarTarea nuevasTareas))
            bucle tareas

        "M" -> do
            putStr "Todas las tareas: \n"
            putStr (unlines (map mostrarTarea tareas))
            bucle tareas

        "U" -> do
            putStr "Tareas con prioridad Alta sin completar: \n"
            let nuevasTareas = filter esUrgente tareas
            putStr (unlines (map mostrarTarea nuevasTareas))
            bucle tareas

        "S" -> return ()

        _ -> do
            putStr "Opcion no válida\n"
            bucle tareas

leerPrioridad "A" = Alta
leerPrioridad "M" = Media
leerPrioridad "B" = Baja
leerPrioridad _ = Baja
        
main:: IO ()
main = bucle []






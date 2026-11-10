#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Uso: $0 directorio"
    exit 1
fi

DIR="$1"

if [ -d "$DIR" ]; then
    cd "$DIR" || exit 1
    rm -rf *

else
    mkdir "$DIR"
    cd "$DIR" || exit 1
fi

mkdir subdir 
touch fichero1
echo "0123456789" > fichero2
ln -s fichero2 enlaceS
ln fichero2 enlaceH

stat *

#Preguntas:  
#
#1. Fichero1 ocupa 0 bloques con tamaño 0 y el fichero2 ocupa 8 bloques 
#   con tamaño 11.
#
#2. Subdir tiene un tamaño de 4096. Al ser un directorio tiene como mínimo
#   dos enlaces, "." y "..".
#
#3. EnlaceH y fichero2 comparten el inodo 530557. Como enlaceH es un enlace
#   duro del fichero2, apunta al mismo inodo que este. Por eso nos muestra el 
#   número de enlaces del inodo como 2. 
#
#4. Al hacer cat de ambos ficheros, se muestra el contenido del fichero2,
#   que contiene 0123456789. Al hacer diff de ambos, no se muestra nada. 
#   Al eliminar fichero2, enlaceH sigue funcionando bien. El inodo no se elimina
#   porque sigue en uso por él. En cambio, enlaceS, que accede al dato a través de 
#   fichero2, pierde la ruta y no funciona. 
#
#5. Se modifican las fechas de acceso, modificación y cambio. Con los flags
#   -a y -m de touch modificamos solo el acceso o la modificación, respectivamente.
#
#
#
#
#
#
#
#
#

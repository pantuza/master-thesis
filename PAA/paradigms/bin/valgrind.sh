#!/bin/bash
ARQ_VG="outputs/valgrind_$1.txt"
CMD_VG="valgrind -v --leak-check=full --show-reachable=yes --leak-resolution=high --log-file=$ARQ_VG"
CHECK_STR="All heap blocks were freed -- no leaks are possible"
if $CMD_VG "bin/$1"; then
  if ! grep "$CHECK_STR" "$ARQ_VG" &> /dev/null; then
    echo "Falha de alocação. Verificar arquivo $ARQ_VG"
    exit 1
  fi
else
  echo "Falha na execução do programa!"
fi

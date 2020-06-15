#!/bin/sh

# usage: sh test.sh

#======COLORS======#

EOC="\033[0m";
GREEN="\033[32m";
YELLOW="\033[33m";
RED="\033[0;31m";
BLUE="\033[0;36m";
HIGH="\033[1m";

clear
cd ..

echo
echo "${HIGH}Author:${EOC}"
cat -e author
ATR1=$(cat -e author | wc -l | tr -d ' ')
ATR2=$(cat -e author | wc -c | tr -d ' ')
if [ $ATR1 -ne '1' ] || [ $ATR2 -lt '6' ]
then
echo "${RED}Author-file\tfailed${EOC}"
else
echo "${GREEN}Author-file\tpassed${EOC}"
fi

NRM1=$(norminette *.c *.h | grep 'Error' | wc -l | tr -d ' ')
if [ $NRM1 -gt '0' ]
then
echo "${RED}Norminette\tfailed${EOC}"
norminette *.c *.h | grep 'Error' > norminette_output
else
echo "${GREEN}Norminette\tpassed${EOC}"
fi

export HOSTTYPE=Testing

MK0=$(make fclean)
MK1=$(make | grep "Nothing" | wc -l | tr -d ' ')
MK2=$(make | grep "Nothing" | wc -l | tr -d ' ')
MK3=$(make clean | grep "Nothing" | wc -l | tr -d ' ')
MK4=$(make | grep "Nothing" | wc -l | tr -d ' ')
MK5=$(make fclean | grep "Nothing" | wc -l | tr -d ' ')
MK6=$(make | grep "Nothing" | wc -l | tr -d ' ')
if [ $MK1 -eq '0' ] && [ $MK2 -eq '1' ] && [ $MK4 -eq '1' ] && [ $MK6 -eq '0' ]
then
echo "${GREEN}Makefile\tpassed${EOC}"
else
echo "${RED}Makefile\tfailed${EOC}"
fi
make clean

cp libft_malloc.so test/libft_malloc.so
cp libft_malloc_Testing.so test/libft_malloc_Testing.so

sleep 1
echo
echo "${HIGH}Compiled library:${EOC}"
ls -la | grep ".so"
LIB=$(ls -la | grep ".so" | wc -l | awk '$1=$1')
if [ $LIB -eq '2' ]; then
echo "${GREEN}Passed${EOC}"
else
echo "${RED}Not passed${EOC}"
fi

sleep 1
echo
echo "${HIGH}Necessary functions:${EOC}"
nm libft_malloc.so | grep -w "_malloc\|_realloc\|_free\|_show_alloc_mem"
NECFUN=$(nm libft_malloc.so | grep -w "_malloc\|_realloc\|_free\|_show_alloc_mem" | wc -l | awk '$1=$1')
if [ $NECFUN -gt '3' ]; then
echo "${GREEN}Passed${EOC}"
else
echo "${RED}Not passed${EOC}"
fi

sleep 1
echo
echo "${HIGH}Functions included from external libraries:${EOC}"
nm -u libft_malloc.so

sleep 1
echo
echo "${HIGH}Compiling tests...${EOC}"
cd test
rm -f test0 test1 test2 test3 test3+ test4 test5 test6
gcc -o test0 test0.c -L. -lft_malloc
gcc -o test1 test1.c -L. -lft_malloc
gcc -o test2 test2.c -L. -lft_malloc
gcc -o test3 test3.c -L. -lft_malloc
gcc -o test3+ test3+.c -L. -lft_malloc
gcc -o test4 test4.c -L. -lft_malloc
gcc -o test5 test5.c -L. -lft_malloc
gcc -o test6 test6.c -L. -lft_malloc

sleep 1
echo
echo "${HIGH}Test malloc:${EOC}"
MLK0=$(/usr/bin/time -l ./test0 2>&1 | grep "page reclaims" | awk '$1=$1' | cut -d ' ' -f1)
MLK1=$(/usr/bin/time -l ./test1 2>&1 | grep "page reclaims" | awk '$1=$1' | cut -d ' ' -f1)
MLK2=$(/usr/bin/time -l ./test2 2>&1 | grep "page reclaims" | awk '$1=$1' | cut -d ' ' -f1)

echo "Malloc not used : ${MLK0} page reclaims"
echo "Malloc used     : ${MLK1} page reclaims"
MLKDIFF=$((MLK1 - MLK0))
if [ $MLKDIFF -lt '255' ]; then
echo "${RED}Difference: ${MLKDIFF}, there was not enough memory allocated, grade 0${EOC}"
elif [ $MLKDIFF -lt '273' ]; then
echo "${GREEN}Difference: ${MLKDIFF}, reasonable overhead, grade 5${EOC}"
elif [ $MLKDIFF -lt '312' ]; then
echo "${YELLOW}Difference: ${MLKDIFF}, big overhead, grade 4${EOC}"
elif [ $MLKDIFF -lt '513' ]; then
echo "${YELLOW}Difference: ${MLKDIFF}, huge overhead, grade 3${EOC}"
elif [ $MLKDIFF -lt '1023' ]; then
echo "${YELLOW}Difference: ${MLKDIFF}, essential overhead, grade 2${EOC}"
elif [ $MLKDIFF -gt '1022' ]; then
echo "${RED}Difference: ${MLKDIFF}, malloc uses one page per allocation, grade 1${EOC}"
fi

sleep 1
echo
echo "${HIGH}Test free:${EOC}"
FREEDIFF1=$((MLK2 - MLK1))
FREEDIFF2=$((MLK2 - MLK0))
echo "Free used       : ${MLK2} page reclaims"
if [ $FREEDIFF1 -le '0' ]; then
echo "${GREEN}Difference: ${MLK2}-${MLK1}=${FREEDIFF1}, less pages reused with free, passed${EOC}"
else
echo "${RED}Difference: ${MLK2}-${MLK1}=${FREEDIFF1}, more pages reused with free, not passed${EOC}"
fi
if [ $MLKDIFF -lt '4' ]; then
echo "${GREEN}Difference: ${MLK2}-${MLK0}=${FREEDIFF2}, good quality of free, passed${EOC}"
else
echo "${YELLOW}Difference: ${MLK2}-${MLK0}=${FREEDIFF2}, bad quality of free, not passed${EOC}"
fi


sleep 1
echo
echo "${HIGH}Test realloc:${EOC}"
REALLOC1=$(./test3 | grep "Bonjours" | wc -l | awk '$1=$1')
./test3
if [ $REALLOC1 -eq '2' ]; then
echo "${GREEN}Passed${EOC}"
else
echo "${RED}Not passed${EOC}"
fi

sleep 1
echo
echo "${HIGH}Test realloc++:${EOC}"
REALLOC2=$(./test3+ | grep "Bonjours" | wc -l | awk '$1=$1')
./test3+
if [ $REALLOC2 -eq '2' ]; then
echo "${GREEN}Passed${EOC}"
else
echo "${RED}Not passed${EOC}"
fi

sleep 1
echo
echo "${HIGH}Error handling:${EOC}"
ERROR=$(./test4 | grep "Bonjours" | wc -l | awk '$1=$1')
./test4
if [ $ERROR -eq '1' ]; then
echo "${GREEN}Passed${EOC}"
else
echo "${RED}Not passed${EOC}"
fi

sleep 1
echo
echo "${HIGH}show_alloc_mem:${EOC}"
SHOW=$(./test5 | grep "TINY\|SMALL\|LARGE\|Total\|bytes" | wc -l | awk '$1=$1')
./test5
if [ $SHOW -eq '9' ]; then
echo "${GREEN}Passed${EOC}"
else
echo "${RED}Not passed${EOC}"
fi

unset HOSTTYPE

#!/bin/bash
#===============================================================================
#
#          FILE:  index.sh
# 
#         USAGE:  ./index.sh 
# 
#   DESCRIPTION:  cscopen and ctags
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR:  amoblin 崔 (), amoblin@gmail.com
#       COMPANY:  bistu
#       VERSION:  1.0
#       CREATED:  2009年03月24日 19时46分16秒 CST
#      REVISION:  ---
#===============================================================================

find . -name "*.h" -o -name "*.c" -o -name "*.cc" > cscope.out

cscope -bqk -i cscope.out

ctags -R

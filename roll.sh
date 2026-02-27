#!/bin/bash
SIDES=${1:-6}
RESULT=$(( (RANDOM % SIDES) + 1 ))
echo "🎲 $SIDES yuzlu zar atildi. Gelen sayi: $RESULT"

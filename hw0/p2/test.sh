for i in {0..5};
  do
    if test -f $i.in;
    then
      ./main < $i.in > $i
    fi
done

for i in {0..5} ;
  do 
    if test -f $i.in;
    then
      echo "comparing "$i.out;
      diff -Z $i.out $i;
    fi
done

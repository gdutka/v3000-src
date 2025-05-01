WORKSPACE=$(dirname $PWD)

while [ ! -d ${WORKSPACE}/BaseTools ]
do
 WORKSPACE=$(dirname $WORKSPACE)
done

echo ${WORKSPACE}

find ./Adders/ -type f -print0 | while read -d $'\0' file;
do 
echo "$file"
base_name=$(basename -- ${file} .v)
aag_name="Adders/ref/${base_name}.aag"
export IN=${file}
export OUT=${aag_name}
envsubst '${IN},${OUT}' < synth.ys > synth_par.ys
yosys synth_par.ys
done

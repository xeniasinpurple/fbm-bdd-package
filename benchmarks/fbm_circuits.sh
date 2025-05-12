find ./FBM_circuits/circuits/ -type f -print0 | while read -d $'\0' file;
do 
echo "$file"
base_name=$(basename -- ${file} .blif)
aag_name="FBM_circuits/opt/${base_name}.aag"
export IN=${file}
export OUT=${aag_name}
envsubst '${IN},${OUT}' < synth_blif.ys > synth_blif_par.ys
yosys synth_blif_par.ys
done

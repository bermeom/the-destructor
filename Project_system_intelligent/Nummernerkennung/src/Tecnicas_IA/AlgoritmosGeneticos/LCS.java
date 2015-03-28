package Tecnicas_IA.AlgoritmosGeneticos;

import java.util.Random;
import java.util.List;
import java.util.Random;

/*
 * @author Juan
 */

public class LCS {
    private int numeroReglas;
    private int parametroAprendizaje;
    private int longitudCromosoma;
    private int logitudFitness;
    private List<Regla> reglas;
    
    public int getNumeroReglas() {
        return numeroReglas;
    }

    public void setNumeroReglas(int numeroReglas) {
        this.numeroReglas = numeroReglas;
    }

    public int getParametroAprendizaje() {
        return parametroAprendizaje;
    }

    public void setParametroAprendizaje(int parametroAprendizaje) {
        this.parametroAprendizaje = parametroAprendizaje;
    }

    public int getLongitudCromosoma() {
        return longitudCromosoma;
    }

    public void setLongitudCromosoma(int longitudCromosoma) {
        this.longitudCromosoma = longitudCromosoma;
    }

    public int getLogitudFitness() {
        return logitudFitness;
    }

    public void setLogitudFitness(int logitudFitness) {
        this.logitudFitness = logitudFitness;
    }
    
    public void inicializarReglasAleatoriamente()
    {
        
    }
}

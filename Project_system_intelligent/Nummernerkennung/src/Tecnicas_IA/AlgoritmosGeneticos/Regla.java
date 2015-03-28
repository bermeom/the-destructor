package Tecnicas_IA.AlgoritmosGeneticos;

import java.util.BitSet;
import java.util.List;

/*
 * @author Juan
 */

public class Regla {
    private BitSet condicion;
    private BitSet accion;
    private int fitness;
    private List<Integer> comodines;

    public Regla(BitSet condicion, BitSet accion, int fitness, List<Integer> comodines) {
        this.condicion = condicion;
        this.accion = accion;
        this.fitness = fitness;
        this.comodines = comodines;
    }
    
    public BitSet getCondicion() {
        return condicion;
    }

    public void setCondicion(BitSet condicion) {
        this.condicion = condicion;
    }

    public BitSet getAccion() {
        return accion;
    }

    public void setAccion(BitSet accion) {
        this.accion = accion;
    }

    public int getFitness() {
        return fitness;
    }

    public void setFitness(int fitness) {
        this.fitness = fitness;
    }

    public List<Integer> getComodines() {
        return comodines;
    }

    public void setComodines(List<Integer> comodines) {
        this.comodines = comodines;
    }
}

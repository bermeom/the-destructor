package Agente;

import Tecnicas_IA.AlgoritmosGeneticos.LCS;
import java.util.BitSet;

/*
 * @author Juan
 */

public class Agente {
   
    private LCS tecnica;
   
    
    public BitSet convertirImagenACromosoma(String imagen)
    {
        BitSet cromosoma = new BitSet(tecnica.getLongitudCromosoma());
        /*Agregar conversion de la imagen en el cromosoma*/
        return cromosoma;
    }
}

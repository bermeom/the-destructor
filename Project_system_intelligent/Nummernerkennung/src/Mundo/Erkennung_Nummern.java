/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Mundo;

import java.util.Random;

/**
 *
 * @author Juan
 */
public class Erkennung_Nummern {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Random r=new Random((long) (Math.random()*10000000));
        System.out.println(r.nextInt());
    }
    
}

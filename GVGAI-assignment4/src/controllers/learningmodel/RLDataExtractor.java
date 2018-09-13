/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package controllers.learningmodel;

import java.util.*;
import tools.*;
import core.game.Observation;
import core.game.StateObservation;

import java.io.FileWriter;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Observable;
import java.util.Vector;

import com.sun.xml.internal.bind.v2.schemagen.xmlschema.List;

import ontology.Types;
import weka.core.Attribute;
import weka.core.FastVector;
import weka.core.Instance;
import weka.core.Instances;

/**
 *
 * @author yuy
 */
public class RLDataExtractor {
    public FileWriter filewriter;
    public static Instances s_datasetHeader = datasetHeader();
    public static int [] dx = {-1, 1, 0, 0};
    public static int [] dy = {0, 0, -1, 1};
    public static int [][] visited = new int[28][32];
    public RLDataExtractor(String filename) throws Exception{
        
        filewriter = new FileWriter(filename+".arff");
        filewriter.write(s_datasetHeader.toString());
        /*
                // ARFF File header
        filewriter.write("@RELATION AliensData\n");
        // Each row denotes the feature attribute
        // In this demo, the features have four dimensions.
        filewriter.write("@ATTRIBUTE gameScore  NUMERIC\n");
        filewriter.write("@ATTRIBUTE avatarSpeed  NUMERIC\n");
        filewriter.write("@ATTRIBUTE avatarHealthPoints NUMERIC\n");
        filewriter.write("@ATTRIBUTE avatarType NUMERIC\n");
        // objects
        for(int y=0; y<14; y++)
            for(int x=0; x<32; x++)
                filewriter.write("@ATTRIBUTE object_at_position_x=" + x + "_y=" + y + " NUMERIC\n");
        // The last row of the ARFF header stands for the classes
        filewriter.write("@ATTRIBUTE Class {0,1,2}\n");
        // The data will recorded in the following.
        filewriter.write("@Data\n");*/
        
    }
    
    public static Instance makeInstance(double[] features, int action, double reward){
        features[7] = action;
        features[8] = reward;
        Instance ins = new Instance(1, features);
        ins.setDataset(s_datasetHeader);
        return ins;
    }
    
    public static double[] featureExtract(StateObservation obs){
        
    	ArrayList<Double> feature = new ArrayList<>();
    	
        Vector2d apos = obs.getAvatarPosition();
        int ax = (int)(apos.x/20), ay = (int)(apos.y/20);
        visited[ax][ay] = 1;
        double[] dist = {9999, 9999, 9999, 9999};
        if (obs.getNPCPositions() != null) {
            for (ArrayList<Observation> l : obs.getNPCPositions()) {
            	if (l.size() > 0) {
	                Observation obser = l.get(0);
	                switch (obser.itype) {
	                    case 14:
	                        dist[0] = Math.abs(obser.position.x - apos.x) + Math.abs(obser.position.y - apos.y);
	                        break;
	                    case 17:
	                        dist[1] = Math.abs(obser.position.x - apos.x) + Math.abs(obser.position.y - apos.y);
	                        break;
	                    case 20:
	                        dist[2] = Math.abs(obser.position.x - apos.x) + Math.abs(obser.position.y - apos.y);
	                        break;
	                    case 23:
	                        dist[3] = Math.abs(obser.position.x - apos.x) + Math.abs(obser.position.y - apos.y);
	                        break;
	                }
            	}
            }
        }
        for (int i = 0; i < 4; i++)feature.add(dist[i]);

        //4
        feature.add((double)obs.getGameTick());
        feature.add((double)obs.getAvatarSpeed());
        feature.add((double)obs.getAvatarHealthPoints());            
        //3
        double [] feature_array = new double[9]; 
        for (int i = 0; i < feature.size(); i++) feature_array[i] = feature.get(i);
        
        return feature_array;
    }
    
    public static Instances datasetHeader(){
        
        if (s_datasetHeader!=null)
            return s_datasetHeader;
        
        FastVector attInfo = new FastVector();

        Attribute att = new Attribute("Dist1"); attInfo.addElement(att);
        att = new Attribute("Dist2"); attInfo.addElement(att);
        att = new Attribute("Dist3"); attInfo.addElement(att);
        att = new Attribute("Dist4"); attInfo.addElement(att);
        
        att = new Attribute("GameTick" ); attInfo.addElement(att);
        att = new Attribute("AvatarSpeed" ); attInfo.addElement(att);
        att = new Attribute("AvatarHealthPoints" ); attInfo.addElement(att);
 
        //action
        FastVector actions = new FastVector();
        actions.addElement("0");
        actions.addElement("1");
        actions.addElement("2");
        actions.addElement("3");
        att = new Attribute("actions", actions);        
        attInfo.addElement(att);
        // Q value
        att = new Attribute("Qvalue");
        attInfo.addElement(att);

        
        Instances instances = new Instances("PacmanQdata", attInfo, 0);
        instances.setClassIndex( instances.numAttributes() - 1);
        
        return instances;
    }
    
}

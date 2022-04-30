#include <Rcpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace Rcpp;

//' Convert a date in format AAAAQQ in standard date format
//'
//' @param datestring a string containing the date in AAAAQQ format
//' @return A Date object
// [[Rcpp::export]]
Date Qdate(std::string datestring) {
  std::string year = datestring.substr(0, 4) ; // récupère l'année
  std::string quarter = datestring.substr(5,1); // récupère le trimestre
  
  int yearint = std::stoi(year);
  int monthint = 3*(std::stoi(quarter) -1) + 1;
  
  return Date(yearint,monthint,1); 
  
}

//' Convert a date vector in format AAAAQQ in standard date format
//'
//' @param datevect a vector of strings containing the dates in AAAAQQ format
//' @return A DateVector object
// [[Rcpp::export]]
std::vector<Date> Qdatevect(std::vector<std::string> datevect) {

  int numitem = datevect.size();  
  std::vector<Date> result ;
  

  
  for (int i = 0; i < numitem ; i++) {
    
    Date temp = Qdate(datevect[i]);
    result.push_back(temp);
    
  }
  
  return result ; 
  
}

//' Add a quarter to a date (in format YYYY-MM-01 with MM in {01, 04, 07, 10})
//'
//' @param currentdate A date for the begining of a quarter
//' @return A Date object
// [[Rcpp::export]]
Date AddQuarter(Date currentdate) {
  
  
  int currentyear = currentdate.getYear() ;
  int currentmonth = currentdate.getMonth();
  
  if (currentmonth < 10) {
    
    return Date(currentyear, currentmonth + 3 , 1);
    
  } else { // cas du passage d'une année
    
    return Date(currentyear +1 , 1 , 1);
    
  }
  
  
  
}

//' Add a quarter to a date (in format AAAAQQ)
//'
//' @param datestring A date (string)
//' @return A string containing the new date
// [[Rcpp::export]]
std::string AddQuarterStr(std::string datestring) {
  
  if (datestring == "NA") {
    
    return "NA";
    
  } else {
    std::string year = datestring.substr(0, 4) ; // récupère l'année
    std::string quarter = datestring.substr(5,1); // récupère le trimestre
  
  int yearint = std::stoi(year);
  int quarterint = std::stoi(quarter);
  
  if (quarterint < 4) {
    
    quarterint += 1;
    
    
  } else {
    
    quarterint = 1;
    yearint +=1 ;
    
  }
  
  year = std::to_string(yearint);
  quarter = std::to_string(quarterint);
  
  return year + "Q" + quarter;
  
  }
  
}


// // [[Rcpp::export]]
// std::vector<std::string> Qdatevec(std::string datestring, int nbquarters) {
// 
//   std::string year = datestring.substr(0, 4) ;
//   std::string quarter = datestring.substr(5,1);
// 
//   int yearint = std::stoi(year);
//   int monthint = 3*(std::stoi(quarter) -1) + 1; // définition du mois associé au début de trimestre
//   
//   std::vector<std::string> date_vect ;
//   
//   
//   for(int i = 1 ; i < nbquarters ; ++i) {
//     
//     Date dtemp = date_vect[i-1];
//     date_vect[i] = AddQuarter(dtemp) ;
//   }
// 
//   return date_vect ;
// 
// 
// }

//' function to split at spaces or eol (internal)
//'
//' @param s A string containing the whole file
//' @return A vector of strings 
// [[Rcpp::export]]
std::vector<std::string> str_split_cpp(const std::string &s) {

  std::vector<std::string> elems ;
  std::stringstream ss(s);
  std::string item;

  while(std::getline(ss, item, '\n')) {
    
    std::vector<std::string> elems1 ;
    std::stringstream ss(item);
    std::string item1;
    
    while(std::getline(ss, item1, '\r')) {
     
      std::vector<std::string> elems2 ;
      std::stringstream ss(item1);
      std::string item2;
     
      while(std::getline(ss, item2, ' ')) {
        
        if (item2.length() > 0) {
          
          elems.push_back(item2);
          
        }
        
      }
    }
    
    elems.push_back("\n");
    
  }
  return elems;
}


//' Read a Troll database in text format
//' The file is split by space character
//'
//' @param filename A string containing the name of the file
//' @return A vector of strings 
// [[Rcpp::export]]
std::vector<std::string> recup_troll_str_vec(std::string filename) {

  // const char eol = '\n';

  const char* fnam = filename.c_str(); // nom du fichier (chaine de caractères)
  
  int n_lines ;


  // début de la lecture du fichier
  Rprintf( "Recuperation de la base au format Troll : %s \n", fnam) ;

  std::vector<std::string> elements ;

  std::ifstream ifs (fnam, std::ifstream::in);

  if (ifs){ // on a pu ouvrir le fichier en lecture

    std::string contents;
    
    std::string line;
    
    while (getline(ifs, line)) {

      contents += line ;
      contents += "\n" ;
    }

    // ifs.seekg(0, std::ios::end);
    // 
    // contents.resize(ifs.tellg());
    // 
    // ifs.seekg(0, std::ios::beg);
    // 
    // ifs.read(&contents[0], contents.size());
    // 
    // // contents : une chaine de caractères unique contenant la totalité du fichier
    // Rprintf( "Taille du fichier : %i \n", contents.size()) ;

    // on subdivise selon les espaces et les retours à la ligne
    elements = str_split_cpp(contents) ;

    n_lines = elements.size() ;
    
    if (n_lines != 0) { // le tableau de chaines n'est pas dégénéré

      // Rprintf( "Nombre de lignes : %i \n", n_lines) ;
    }


  } else {

    // erreur à l'ouverture du fichier
    stop("error opening file");

  }

  //  fclose(filepointer);
  ifs.close() ;



  return elements;
}


//' Merge two dataframes based on the date column 
//'
//' @param left a data.frame with a date column
//' @param right a data.frame with a date column
//' @return A data.frame
// [[Rcpp::export]]
DataFrame dateMerge(DataFrame left, 
                    DataFrame right) {
  
  if (left.containsElementNamed("date") && 
      right.containsElementNamed("date")) {
    
    // voir les conversions de type (date / string) avant la fusion
    
    // dates de chaque dataframe
    std::vector<Date> date_left = left["date"];
    std::vector<Date> date_right = right["date"];
    
    int nmax_left = date_left.size() ;
    int nmax_right = date_right.size();
    
    // détermination de la réunion des dates
    if (nmax_left > 0 &&
        nmax_right > 0) {
      
      Date datemin_left = date_left[0] ; // premiere date de la table de gauche
      Date datemin_right = date_right[0] ; // première date de la table de droite
      
      // Rprintf("datemin_left : %s \n",datemin_left.format().c_str());
      // Rprintf("datemin_right : %s \n",datemin_right.format().c_str());
      
      Date datemax_left = date_left[nmax_left -1] ; // dernière date de la table de gauche
      Date datemax_right = date_right[nmax_right -1] ; // dernière date de la table de droite
      
      // Rprintf("datemax_left : %s \n",datemax_left.format().c_str());
      // Rprintf("datemax_right : %s \n",datemax_right.format().c_str());
      
      Date datemin ;
      Date datemax ;
      
      if (datemin_left < datemin_right) {
        datemin = datemin_left ;
      } else {
        datemin= datemin_right ;
      }
      
      // Rprintf("datemin : %s \n",datemin.format().c_str());
      
      if (datemax_left > datemax_right) {
        datemax= datemax_left ;
      } else {
        datemax = datemax_right;
      }
      
      // Rprintf("datemax : %s \n",datemax.format().c_str());
      
      std::vector<Date> datetemp;
      
      while (datemin <= datemax) {
        datetemp.push_back(datemin) ;        
        datemin = AddQuarter(datemin);
      }
      
      int nbrows = datetemp.size() ; // nombre de dates dans la jointure
      
      // dataframe contenant la jointure par la date
      DataFrame result = DataFrame::create(Named("date") = datetemp) ;
      
      // data.frame de gauche
      CharacterVector colnames_left = left.names();
      int ncol_left = left.ncol();
      
      for (int i=0 ; i < ncol_left ; i++) {
        
        std::string strtemp = std::string(colnames_left[i]);
        NumericVector valuetemp = left[i];
        
        
        
        if (strtemp != "date") {
          
          // Rprintf("%s \n", strtemp.c_str());
          // Rprintf("nb de dates dans la jointure: %i \n", nbrows);
          
          // réécrire les valeurs ici aux bonnes dates
          
          NumericVector tempnum (nbrows) ;
          
          int ileft = 0;
          
          for (int imerge = 0 ; imerge < nbrows ; imerge++) {
            
            if (datetemp[imerge] < date_left[0]) {
              // trop tot
              
              // Rprintf("trop tot : %i \n", imerge);
              tempnum[imerge] = NA_REAL ;
              
              continue ;
              
            } else if (datetemp[imerge] > date_left[nmax_left -1] ) {
              // trop tard
              
              // Rprintf("trop tard : %i \n", imerge);
              tempnum[imerge] =  NA_REAL;
              
              continue ;
              
            } else {
              
              // Rprintf(" %i <-> %i \n", imerge,ileft);
              tempnum[imerge] = valuetemp[ileft];
              
              ileft += 1;
              continue ;
              
            }
            
          }
          
          result.push_back(tempnum,strtemp.c_str()) ;
          
        }
        
      }
      
      // data.frame de droite
      
      CharacterVector colnames_right = right.names();
      int ncol_right = right.ncol();
      
      for (int i=0 ; i < ncol_right ; i++) {
        
        std::string strtemp = std::string(colnames_right[i]);
        NumericVector valuetemp = right[i];
        
        
        
        if (strtemp != "date") {
          
          // Rprintf("%s \n", strtemp.c_str());
          // Rprintf("nb de dates dans la jointure: %i \n", nbrows);
          
          // réécrire les valeurs ici aux bonnes dates
          
          NumericVector tempnum (nbrows) ;
          
          int iright = 0;
          
          for (int imerge = 0 ; imerge < nbrows ; imerge++) {
            
            if (datetemp[imerge] < date_right[0]) {
              // trop tot
              
              // Rprintf("trop tot : %i \n", imerge);
              tempnum[imerge] = NA_REAL ;
              
              continue ;
              
            } else if (datetemp[imerge] > date_right[nmax_right -1] ) {
              // trop tard
              
              // Rprintf("trop tard : %i \n", imerge);
              tempnum[imerge] =  NA_REAL;
              
              continue ;
              
            } else {
              
              // Rprintf(" %i <-> %i \n", imerge,iright);
              tempnum[imerge] = valuetemp[iright];
              
              iright += 1;
              continue ;
              
            }
            
          }
          
          result.push_back(tempnum,strtemp.c_str()) ;
          
        }
        
      }
      
      return result ;
      
      
    }
    
    stop("Invalid dataframe: no lines");  
    
  }
  
  stop("Invalid dataframe: no date column");  
  
}

//' Read a Troll database in text format
//' The file is split by space character
//'
//' @param str_vec A vector of strings containing the data
//' @return A List
// [[Rcpp::export]]
List recup_troll(std::vector<std::string> str_vec) {
  
  int idx_ligne = 0; // index de ligne (augmente avec \n)
  int idx_elem = 0; // index d'élément (plusieurs éléments par ligne)
  int n_elem = str_vec.size() ; // nombre d'éléments dans le tableau 
  
  // int idx_obs = 0; // observations pour la variable en cours
  int nobs_loc = 0; // nombre d'observations
  int nb_comment = 0; // nombre de lignes de commentaires
  
  // int nb_dates = 0; // nombre de trimestres à lire dans le cas d'une série temporelle
  
  std::vector<std::string> nom_var_ts;
  std::vector<std::string> nom_var_coeff;
  
  std::string nom_var ; // nom de la variable courante
  std::string date_loc ; // date courante
  
  NumericVector df_loc(0) ;
  std::vector<Date> df_loc_dates(0) ; // pour le vecteur des dates courantes
  // std::string date_temp ;
  
  // structures de données pour le résultat de la lecture
  List listcoef = List::create(); // pour contenir les coefficients
  List listts = List::create();
  
  bool is_timeserie ;
  
  while ((idx_elem < n_elem) ) {
    
    // Rprintf("Element %i : %s \n",idx_elem,str_vec[idx_elem].c_str());
    
    if (str_vec[idx_elem] == "\n" ) { // on passe une ligne
      
      idx_ligne += 1; 
      idx_elem +=1 ;
      continue; // on saute le reste des cas  
      
    }
    
    else if (str_vec[idx_elem] == "NAME:") { // définition d'un nouvel élément

      if (idx_ligne > 0) { // pour gérer le début de fichier

        if (is_timeserie) {
            // on ajoute une série temp
            // DataFrame dftemp = DataFrame::create(Named("date") = df_loc_dates,
            DataFrame dftemp = DataFrame::create(Named("date") = df_loc_dates,
                                                 Named(nom_var) = df_loc) ;

          //dftemp.attr("row.names") = df_loc_dates ;
          listts.push_back(dftemp,nom_var);

        } else {
          // on ajoute un coefficient
          listcoef.push_back(df_loc, nom_var);
          
        }

      }

      nom_var = str_vec[idx_elem + 1]; // déclaration du nom de la variable courante
      // Rprintf("Element %i : %s \n",idx_elem+1,str_vec[idx_elem+1].c_str());
      idx_elem +=2 ; // on passe deux cases
      
      continue; // on saute le reste des cas

    }

    else if (str_vec[idx_elem] == "SPECS:") {

      // remise à zéro des variables locales
      df_loc = NumericVector(0); // pour la lecture des valeurs numériques
      df_loc_dates.resize(0) ; // pour la lecture des dates
      
      date_loc = str_vec[idx_elem + 2]; // pour la date de début de la série temp
      
      // Rprintf("Element %i : %s \n",idx_elem + 2,str_vec[idx_elem+2].c_str());
      
      is_timeserie = (date_loc != "NA"); // est ce qu'on a une série temp ou un coeff ?

      if (is_timeserie) { // cas d'une série temp
        
        // nb_dates = std::stoi(str_vec[idx_elem + 4]); // on lit le nombre de dates
        
        // date_temp = Qdate(date_debut_loc); // on instancie la première date
        
        
        nom_var_ts.push_back(nom_var); // on ajoute le nom à la liste

      }
      else { // sinon c'est un coeff

        
        nom_var_coeff.push_back(nom_var); // on ajoute le nom à la liste

      }


      nobs_loc = std::stoi(str_vec[idx_elem + 4]) ; // attention au cas où c'est un coeff
      // Rprintf("Element %i : %s \n",idx_elem + 4 ,str_vec[idx_elem + 4].c_str());
      
      nb_comment = std::stoi(str_vec[idx_elem + 6]); // nombre de ligne à sauter pour ne pas lire les commentaires
      // Rprintf("Element %i : %s \n",idx_elem + 6,str_vec[idx_elem + 6].c_str());

      idx_elem += 7 ;  // on saute les six éléments qu'on vient de lire

      int idx_ligne_temp = 0;
      while (idx_ligne_temp < nb_comment +1) {

        if (str_vec[idx_elem] == "\n" ) { // on passe une ligne

          idx_ligne_temp += 1;
          idx_ligne += 1;

        }
        // Rprintf("Element %i : %s \n",idx_elem,str_vec[idx_elem].c_str());
        idx_elem += 1; // on avance d'un élément

      }
      
      continue ; // on saute le reste des cas
    }


    else { // on est sur une  valeur
      
      if (strcmp(str_vec[idx_elem].c_str(),"NA") ==0 ) {
        // cas d'un NA
        df_loc.push_back(NA_REAL) ;
        
        
      } else {
        
        df_loc.push_back(std::stod(str_vec[idx_elem]));
        
      }
      
      if (is_timeserie) {
        // pour stocker les dates de la série temp
        df_loc_dates.push_back(Qdate(date_loc)) ;
        
      }
      
      idx_elem += 1;
      
      date_loc = AddQuarterStr(date_loc) ;

      }

    }
  
  if (is_timeserie) {
    
    // on ajoute une série temp
    // DataFrame dftemp = DataFrame::create(Named("date") = df_loc_dates,
    DataFrame dftemp = DataFrame::create(Named("date") = df_loc_dates,
                                         Named(nom_var) = df_loc) ;
    
    //dftemp.attr("row.names") = df_loc_dates ;
    listts.push_back(dftemp,nom_var);
  } else {
    // on ajoute un coefficient
    listcoef.push_back(df_loc, nom_var);
    
  }
  
  // Rprintf("Nombre de lignes : %i  \n",idx_ligne);
  
  
  List result  = List::create(Rcpp::Named("var_ts") = listts,
                              Rcpp::Named("var_coeff") = Rcpp::DataFrame(listcoef)) ;
  
  return result ;

  }

// A faire : convertir les différents data frames en un seul via datemerge

//' Read a Troll database in text format
//'
//' @param filename A vector of strings containing the data
//' @return A List
// [[Rcpp::export]]
List recup_troll_rcpp(std::string filename) {
  
  return recup_troll(recup_troll_str_vec(filename));
  
}






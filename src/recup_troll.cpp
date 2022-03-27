#include <Rcpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace Rcpp;

//internal function to split at spaces or eol
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



// [[Rcpp::export]]
std::vector<std::string> recup_troll_str_vec(std::string filename) {

  const char eol = '\n';

  const char* fnam = filename.c_str();
  
  int n_lines ;


  // pointeur sur le fichier
  //  FILE *filepointer = fopen(filename.c_str(), "r");

  //  if (filepointer == NULL) {
  //    stop("error opening file");
  //  }

  // début de la lecture du fichier
  Rprintf( "Recuperation de la base au format Troll : %s \n", fnam) ;

  std::vector<std::string> elements ;

  std::ifstream ifs (fnam, std::ifstream::in);

  if (ifs){ // on a pu ouvrir le fichier en lecture

    std::string contents;

    ifs.seekg(0, std::ios::end);

    contents.resize(ifs.tellg());

    ifs.seekg(0, std::ios::beg);

    ifs.read(&contents[0], contents.size());

    // contents : une chaine de caractères unique contenant la totalité du fichier
    Rprintf( "Taille du fichier : %i \n", contents.size()) ;

    // on subdivise selon les espaces et les retours à la ligne
    elements = str_split_cpp(contents) ;

    n_lines = elements.size() ;
    
    if (n_lines != 0) { // le tableau de chaines n'est pas dégénéré

      Rprintf( "Nombre de lignes : %i \n", n_lines) ;
    }


  } else {

    // erreur à l'ouverture du fichier
    stop("error opening file");

  }

  //  fclose(filepointer);
  ifs.close() ;
  

  // for(int i = n_lines -20; i < n_lines; i++) {
  //   
  //   Rprintf( " %s \n", elements[i].c_str()) ;
  //   
  // }


  return elements;
}


List recup_troll(std::vector<std::string> str_vec) {
  
  
  List result  = List::create(Rcpp::Named("result") = 0) ;
}




// 
// 
// 
// # on boucle sur les lignes
// idx_ligne <- 1
// 
// liste_nom_var_ts <- NULL
// liste_nom_var_coef <- NULL
// 
// # date_debut <- NULL
// 
// # nb_observ <- NULL
// 
// dfts <- c()
//   
//   dfcoef <- c()
//   
// # on traite ligne à ligne le fichier
//   
//   while(idx_ligne <= linestot) {
//     
//     ligne <- filesplit[[idx_ligne]] 
//     
//     
//     
//     if (ligne[1] == "NAME:") {
//       
//       if (idx_ligne > 1) {
//         
// # cat("Nombre d'observations :", idx_obs, "\n")
//         
//         if (is_timeserie) {
//           
//           annee_trim <- stringr::str_split(date_debut_loc,"Q")
//           
//           annee <- annee_trim[[1]][1]
//           trim <- annee_trim[[1]][2]
//           
//           dfts <- cbind(dfts,ts(df_loc,
//                                 frequency = 4,
//                                 start = c(as.integer(annee),as.integer(trim))))
//           
//         } else {
//           
//           dfcoef <- cbind(dfcoef,df_loc)
//         }
//         
//         
//       }
//       
//       
//       nom_var <- ligne[2]
//       
// # cat("Variable :",nom_var,"\n")
//       
//       
//       idx_obs <- 1  # pour l'insertion à la bonne ligne
//         
//         nobs_loc <- 0
//       df_loc <- c()
//         
//         idx_ligne <- idx_ligne +1
//       
//     } 
//     else if (ligne[1] == "SPECS:") {
//       
//       type_donnee <- ligne[2]
//       
//       date_debut_loc <- ligne[3]
//       
//       is_timeserie <- ligne[3] != "NA"
//       
//       if (is_timeserie) {
//         
//         liste_nom_var_ts <- c(liste_nom_var_ts,nom_var)
//         
//       } else {
//         
//         liste_nom_var_coef <- c(liste_nom_var_coef,nom_var)
//         
//       }
//       
// # date_debut <- c(date_debut,date_debut_loc)
//       
//       nobs_loc <- as.numeric(ligne[5])
//         
// # nb_observ <- c(nb_observ, nobs_loc)
//         
//         df_loc <- rep(NA,nobs_loc)
//           
// # cat("Date début :", date_debut_loc,"\n")
// # cat("Nombre de données :", nobs_loc,"\n" )
//           
//           nb_comment <- as.numeric(ligne[7]) # nombre de ligne à sauter pour ne pas lire les commentaires
//             
//             idx_ligne <- idx_ligne + 1 + nb_comment
//             
//     }
//     
//     else {
// # on est sur une ligne avec des valeurs
//       for (item in ligne) {
//         
//         if (nchar(item)>0) {
//           
//           if (item != "NA") {
//             df_loc[idx_obs] <- as.numeric(item)
//           } else {
//             
//             df_loc[idx_obs] <- NA
//           }
//           
//           idx_obs <- idx_obs +1
//           
//         }
//         
//       }
//       
//       
//       idx_ligne <- idx_ligne +1
//     }
//     
//     
//   }
//   
//   if (idx_ligne > 1) {
//     
// # cat("Nombre d'observations :", idx_obs, "\n")
//     
//     if (is_timeserie) {
//       
//       annee_trim <- stringr::str_split(date_debut_loc,"Q")
//       
//       annee <- annee_trim[[1]][1]
//       trim <- annee_trim[[1]][2]
//       
//       dfts <- cbind(dfts,ts(df_loc,
//                             frequency = 4,
//                             start = c(as.integer(annee),as.integer(trim))))
//       
//     } else {
//       
//       dfcoef <- cbind(dfcoef,df_loc)
//     }
//     
//     
//   }
//   
//   colnames(dfts) <- liste_nom_var_ts
//     colnames(dfcoef) <- liste_nom_var_coef
//     
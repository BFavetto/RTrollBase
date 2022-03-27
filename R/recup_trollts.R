#' Fonction de lecture des bases de données issues de Troll
#'
#' @param filename nom du fichier .frm au format trolltxt (chaine de caractere)
#' 
#' @import stringr, zoo
#' 
#' @return
#' 
#' @export
#'
#' @examples
recup_trollts <- function(filename) {
  
  # récupération des bases de données troll au format trolltxt
  
  lecture_readLines <- readLines(filename)
  
  # séparation de chaque ligne selon le caractère " "
  filesplit <- stringr::str_split(lecture_readLines, pattern = " ")
  
  linestot <- length(filesplit) # nb de lignes dans le fichier
  
  # on boucle sur les lignes
  idx_ligne <- 1
  
  liste_nom_var_ts <- NULL
  liste_nom_var_coef <- NULL
  
  # date_debut <- NULL
  
  # nb_observ <- NULL
  
  dfts <- c()
  
  dfcoef <- c()
  
  # on traite ligne à ligne le fichier
  
  while(idx_ligne <= linestot) {
    
    ligne <- filesplit[[idx_ligne]] 
    
    
    
    if (ligne[1] == "NAME:") {
      
      if (idx_ligne > 1) {
        
        # cat("Nombre d'observations :", idx_obs, "\n")
        
        if (is_timeserie) {
          
          annee_trim <- stringr::str_split(date_debut_loc,"Q")
          
          annee <- annee_trim[[1]][1]
          trim <- annee_trim[[1]][2]
          
          dfts <- cbind(dfts,ts(df_loc,
                                frequency = 4,
                                start = c(as.integer(annee),as.integer(trim))))
          
        } else {
          
          dfcoef <- cbind(dfcoef,df_loc)
        }
        
        
      }
      
      
      nom_var <- ligne[2]
      
      # cat("Variable :",nom_var,"\n")
      
      
      idx_obs <- 1  # pour l'insertion à la bonne ligne
      
      nobs_loc <- 0
      df_loc <- c()
      
      idx_ligne <- idx_ligne +1
      
    } 
    else if (ligne[1] == "SPECS:") {
      
      type_donnee <- ligne[2]
      
      date_debut_loc <- ligne[3]
      
      is_timeserie <- ligne[3] != "NA"
      
      if (is_timeserie) {
        
        liste_nom_var_ts <- c(liste_nom_var_ts,nom_var)
        
      } else {
        
        liste_nom_var_coef <- c(liste_nom_var_coef,nom_var)
        
      }
      
      # date_debut <- c(date_debut,date_debut_loc)
      
      nobs_loc <- as.numeric(ligne[5])
      
      # nb_observ <- c(nb_observ, nobs_loc)
      
      df_loc <- rep(NA,nobs_loc)
      
      # cat("Date début :", date_debut_loc,"\n")
      # cat("Nombre de données :", nobs_loc,"\n" )
      
      nb_comment <- as.numeric(ligne[7]) # nombre de ligne à sauter pour ne pas lire les commentaires
      
      idx_ligne <- idx_ligne + 1 + nb_comment
      
    }
    
    else {
      # on est sur une ligne avec des valeurs
      for (item in ligne) {
        
        if (nchar(item)>0) {
          
          if (item != "NA") {
            df_loc[idx_obs] <- as.numeric(item)
          } else {
            
            df_loc[idx_obs] <- NA
          }
          
          idx_obs <- idx_obs +1
          
        }
        
      }
      
      
      idx_ligne <- idx_ligne +1
    }
    
    
  }
  
  if (idx_ligne > 1) {
    
    # cat("Nombre d'observations :", idx_obs, "\n")
    
    if (is_timeserie) {
      
      annee_trim <- stringr::str_split(date_debut_loc,"Q")
      
      annee <- annee_trim[[1]][1]
      trim <- annee_trim[[1]][2]
      
      dfts <- cbind(dfts,ts(df_loc,
                            frequency = 4,
                            start = c(as.integer(annee),as.integer(trim))))
      
    } else {
      
      dfcoef <- cbind(dfcoef,df_loc)
    }
    
    
  }
  
  colnames(dfts) <- liste_nom_var_ts
  colnames(dfcoef) <- liste_nom_var_coef
  
  
  
  return(list(as.data.frame(zoo::as.zoo(dfts)),
              as.data.frame(dfcoef)))
  
}
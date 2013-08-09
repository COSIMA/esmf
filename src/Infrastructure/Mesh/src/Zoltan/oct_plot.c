/*****************************************************************************
 * Zoltan Library for Parallel Applications                                  *
 * Copyright (c) 2000,2001,2002, Sandia National Laboratories.               *
 * This software is distributed under the GNU Lesser General Public License. *
 * For more info, see the README file in the top-level Zoltan directory.     *
 *****************************************************************************/
/*****************************************************************************
 * CVS File Information :
 *    $RCSfile: oct_plot.c,v $
 *    $Author: dneckels $
 *    $Date: 2007/11/28 16:13:52 $
 *    Revision: 1.2 $
 ****************************************************************************/
#include "zz_const.h"
#include "octree_const.h"
#include "rootlist_const.h"

#ifdef __cplusplus
/* if C++, define the rest of this header file as extern C */
extern "C" {
#endif




/* Routine to print the SFC generated by the octree partitioning.
 * Currently, this routine works only for 2D problems.
 * Extension to 3D is straightforward, but harder to plot.
 *
 * This routine is SERIAL and, thus, does not scale with the number 
 * of processors.
 *
 * Three types of output are produced by this one routine.
 *
 * Region information:  For each region, its coordinates are printed, 
 * along with the processor number of the processor storing the region.
 * To create a curve through the regions, 
 * -  redirect output of zdrive to file
 * -  grep PLOTREG file > file.r
 * -  Edit file.r so that processor's blocks of regions are in increasing
 *    order (i.e., all processor zero's regions, then all processor one's, etc.
 *    If the architecture handles Print_Sync correctly, this step should not
 *    be needed.
 * -  Edit file.r to remove PLOTREG from each line; in vi: %s/PLOTREG//g
 * -  gnuplot: plot "file.r" using 2:3 with linespoints
 *
 *
 * SFC through octants:  For each leaf octant, its midpoint is printed, along
 * with the processor number of the processor storing the octant. 
 * NOTE:  the curve through the octants is what is used for partitioning!
 * To create a curve through the octants, 
 * -  redirect output of zdrive to file
 * -  grep PLOTOCT file > file.o
 * -  Edit file.o so that processor's blocks of octants are in increasing
 *    order (i.e., all processor zero's octants, then all processor one's, etc.
 *    If the architecture handles Print_Sync correctly, this step should not
 *    be needed.
 * -  Edit file.o to remove PLOTOCT from each line; in vi: %s/PLOTOCT//g
 * -  gnuplot: plot "file.o" using 2:3 with linespoints
 *
 *
 * The resulting octree:  For each leaf octant, its bounding box is written
 * to file "octants.gnu".  Writing to the file must be done serially.
 * To plot the octree:
 * - gnuplot: plot "octants.gnu" with lines
 */


void Zoltan_Oct_Plots(ZZ *zz) {
pRList  RootList;                           /* list of all local roots */
pOctant RootOct;
OCT_Global_Info *OCT_info = (OCT_Global_Info *)(zz->LB.Data_Structure);
FILE *fp;
pRegion tmp;

  Zoltan_Print_Sync_Start(zz->Communicator, 1);

  if (zz->Proc == 0)
    fp = fopen("octants.gnu", "w");
  else
    fp = fopen("octants.gnu", "a");
  
  RootList = Zoltan_Oct_POct_localroots(OCT_info);
  while((RootOct = RL_nextRootOctant(&RootList))) {
    while(RootOct) {
      if(Zoltan_Oct_isTerminal(RootOct)) {

        /* Octant is terminal; surf the region list and print regions. */
        tmp = RootOct->list;
        while (tmp != NULL) {
          printf("%d PLOTREG %f %f\n", zz->Proc, tmp->Coord[0], tmp->Coord[1]);
          tmp = tmp->next;
        }

        /* Print the octant midpoint */
        printf("%d PLOTOCT %f %f\n", zz->Proc, 
               0.5 * (RootOct->min[0] + RootOct->max[0]),
               0.5 * (RootOct->min[1] + RootOct->max[1]));

        /* Print the octant bounding box */
        fprintf(fp, "%f %f\n", RootOct->min[0], RootOct->min[1]);
        fprintf(fp, "%f %f\n", RootOct->max[0], RootOct->min[1]);
        fprintf(fp, "%f %f\n", RootOct->max[0], RootOct->max[1]);
        fprintf(fp, "%f %f\n", RootOct->min[0], RootOct->max[1]);
        fprintf(fp, "%f %f\n\n", RootOct->min[0], RootOct->min[1]);
      }
      RootOct = Zoltan_Oct_POct_nextDfs(OCT_info, RootOct);
    }
  }

  fclose(fp);
  Zoltan_Print_Sync_End(zz->Communicator, 1);
}

#ifdef __cplusplus
} /* closing bracket for extern "C" */
#endif

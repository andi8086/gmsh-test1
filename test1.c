#include <gmshc.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>


double lc_callback(int dim, int tag, double x, double y, double z,
                   double lc, void *data)
{
        return 0.001 + 0.1*sqrt((x-0.5)*(x-0.5) + (y-0.5)*(y-0.5));
}


int main(int argc, char **argv)
{

        int ierr;

        gmshInitialize(argc, argv, 1, 0, &ierr);

        
        gmshModelAdd("t1", &ierr);

        const double lc = 0.1;

        gmshModelGeoAddPoint(0, 0, 0, lc, 1, &ierr);
        gmshModelGeoAddPoint(1, 0, 0, lc, 2, &ierr);
        gmshModelGeoAddPoint(1, 1, 0, lc, 3, &ierr);
        gmshModelGeoAddPoint(0, 1, 0, lc, 4, &ierr);

        gmshModelGeoAddLine(1, 2, 1, &ierr);
        gmshModelGeoAddLine(2, 3, 2, &ierr);
        gmshModelGeoAddLine(3, 4, 3, &ierr);
        gmshModelGeoAddLine(4, 1, 4, &ierr);

        const int cl1[] = {4, 1, 2, 3};
        gmshModelGeoAddCurveLoop(cl1, 4, 1, 0, &ierr);

        const int s1[] = {1};
        gmshModelGeoAddPlaneSurface(s1, 1, 1, &ierr);

        gmshModelGeoSynchronize(&ierr);
      
        gmshModelMeshSetSizeCallback(lc_callback, NULL, &ierr);
        

        // 2 = 2D mesh
        gmshModelMeshGenerate(2, &ierr);
        //        printf("err = %d\n", ierr);


        gmshWrite("t1.msh", &ierr);

        int *eleTypes;
        size_t eleTypes_n;
        gmshModelMeshGetElementTypes(&eleTypes, &eleTypes_n, 2, -1, &ierr);
        printf("err = %d\n", ierr);

        size_t *ntags;
        size_t ntags_n;
        double *coords;
        size_t coords_n;

        gmshModelMeshGetNodesByElementType(eleTypes[0],
                &ntags, &ntags_n,
                &coords, &coords_n,
                NULL, NULL,
                -1,
                false,
                &ierr);
        printf("coords_n = %lu\n", coords_n);
        for (size_t j = 0; j < coords_n / 9; j++) {
                printf("(%lf, %lf)\n", coords[j*9], coords[j*9+1]);
                printf("(%lf, %lf)\n", coords[j*9+3], coords[j*9+4]);
                printf("(%lf, %lf)\n", coords[j*9+6], coords[j*9+7]);
                printf("\n");
        }
        

        gmshFltkRun(&ierr);

        gmshFinalize(&ierr);
        return 0;
}

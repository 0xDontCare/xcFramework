// unit test for xString_countChar function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    unsigned char sub;
    int expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"fbCEBebf\0FaBedBaDAfaadAe\0\0FecCBbEdDbbFbaCfCfdF\0\nbd\nDb\nbcaECd", 60, 'D', 3},
        {"E\n\ncBeeADc\nBffBDFCDaacFba\0FBbefaaaeB\0A\ndd\n\0ECAddbcFbaFfEEb\nf", 60, 'D', 3},
        {"FBbEDadddBddfdf\0Aa\nCCBbdCC\0FF\n\0BBDCeFBfBbCeAbECcA\nAeEeDbCBed", 60, 'c', 1},
        {"Ff\0F\neCDAf\0DdcECEeddEFf\nEcBcAdef\naD\nCcdAEFBeA\0dFFaBd\0FFdCBD\n", 60, '\0', 4},
        {"dfBfbEEEefbFeEFfabde\nD\0ba\nFdEACdB\nE\nCaFbFcAAE\0Df\nCDFFaAeEAec", 60, 'E', 8},
        {"BcD\0bDd\nDFf\0aeEDd\0CDdDdDCdf\0\n\0cCDF\nBBacCeCcCaDecfef\nccbdeBed", 60, 'F', 2},
        {"\nFEbCedDAB\0b\nACDABEFcbBceBb\n\ncbdBfdbAb\nFDdfcfEaFdB\0\0BABffcac", 60, 'c', 6},
        {"Aa\nf\nFCAe\0BD\nbf\nFdA\0bBFD\nFbD\0dfC\0fbDcacbAfCa\0\nADBbfefFfEcAFc", 60, 'D', 5},
        {"B\nce\naCFca\nadEFbcdEEFBFf\ndCbbDdfA\0FDAAcAeabcBaaBEAB\0CEEeB\0\ne", 60, '\n', 5},
        {"EFCCbaf\0\0FedcFB\n\neEDaAFAC\0bFAaecA\n\naCCA\0EdcbCeb\ndBfaFeBBAa\n\0", 60, '\0', 5},
        {"aCEcdbaDcfA\n\0CcfededbcFBAdcf\0caFCDb\ndff\n\nEAcD\0bdefFcBffEF\n\nE", 60, 'D', 3},
        {"aef\na\nC\0b\0AcabddbEFBD\0BDccBdDfadf\0eCDD\0acCeAf\0fBEdaAcfb\ncfCd", 60, 'a', 6},
        {"cceBaBEeDAde\nAcdC\nbaCE\nacbe\nb\0edFCE\nbBCFf\0AEFEC\nAfea\n\nD\nfbFB", 60, 'e', 6},
        {"ccFEDDcbedBcadbfeC\0\0\0\0ACaACeaedf\nfA\nbc\nFdE\n\ndDACEDaaA\nFCcEcd", 60, 'B', 1},
        {"DccAfCFadFEddfF\0c\nFEEfbCdDECBFCEab\0DFABaFd\nCf\nfEADfaFc\0AC\0Fd", 60, 'A', 4},
        {"dFeCadadbccBbbaC\0\nc\0\0DCceDfbAdfF\neEaffcbcebBebffFbBb\0fFDCdfF", 60, 'c', 6},
        {"FbcEd\nfbfdFcABFDAfbbb\nEAeFceAB\0FFFfafffAe\nDeefFE\nCdeCbBFfE\nE", 60, 'c', 3},
        {"aabEcdFc\0Eba\nbEEbBfaEf\0AfefBcbeEdEaEEa\0CAAc\n\0aaabccCe\nbCFaDE", 60, 'a', 10},
        {"afeCc\nfbBEa\naA\0bF\0BCadcfbDcBd\n\n\0fccCFACbbACbAAEcafdAcd\0\nFCaA", 60, 'b', 6},
        {"dbbF\nd\0B\0fACbfbaFCcCcd\nACFdedaccBcDcfadECEcfbFCDBCEfaBc\nAeCE", 60, 'e', 2},
        {"DffeFEbcD\nbeAAC\0deCbeaaFC\0DAACceBC\0fAbAbda\ncf\0\0Aafeb\0cBDCadd", 60, 'e', 6},
        {"FcDFeDbcB\0EeFDEDeeDCcADcbBBFAafCefDAdADBb\0F\nEdA\naCB\n\n\ne\nCACC", 60, 'b', 3},
        {"adcCf\nFEefedcaCDbFebdFcCEdEcD\nfcdA\nCfDAA\0ae\nd\0EefCEa\nFCfCBcd", 60, 'E', 5},
        {"EbEacEbBDFD\0fCec\nCBaADCfeEcEcdfDeAe\0\0bfFEaCfAbBFFcbfeBFceCAA", 60, 'D', 4},
        {"\0CB\nACFEDfe\nCcDcFEAfB\0dEFb\n\0ABFDFCCFaCcfFcaBadAabDecbDCbBebe", 60, 'd', 2},
        {"fcccdEaEECF\0\nAcC\nb\n\0dAbc\0Ca\nFC\0eAfb\ndAfaFc\0CfFBbCcb\nDFcBBdcB", 60, 'b', 5},
        {"bC\0eBDbFdCddEDf\n\0EA\naFAB\n\0fEFcfBDbfcbFe\0fcf\0ffBAd\0\0af\n\nC\nFF\n", 60, 'E', 3},
        {"DBe\0AbDaA\0cAb\0DaddbB\nCEFBCCaCceAccA\ncdCcacDE\neaAbdFCc\0DdcCFa", 60, 'b', 4},
        {"d\0\nBB\0eC\nEeFBfae\0EFCfBDafCBe\ncFaC\ndFE\0BdAeCeFFFfaCccEDfeaEeF", 60, 'b', 0},
        {"EACCcecAFBc\0\nEdBcdE\n\0e\0EFe\0A\0bBfFcCCDef\ndCfAbeebDfB\0eFcafcEd", 60, 'c', 7},
        {"", 0, 'a', 0}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++){
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works correctly -> no assertions placed

        int actual = xString_countChar(str, test_cases[i].sub);
        assert(actual == test_cases[i].expected && "xString_countChar failed");

        xString_free(str);
        printf("Test %d passed!\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}
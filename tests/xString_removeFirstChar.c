// unit test for xString_removeFirstChar function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    unsigned char sub;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"fbCEBebf\0FaBedBaDAfaadAe\0\0FecCBbEdDbbFbaCfCfdF\0\nbd\nDb\nbcaECd", 60, 'D', "fbCEBebf\0FaBedBaAfaadAe\0\0FecCBbEdDbbFbaCfCfdF\0\nbd\nDb\nbcaECd", 59},
        {"E\n\ncBeeADc\nBffBDFCDaacFba\0FBbefaaaeB\0A\ndd\n\0ECAddbcFbaFfEEb\nf", 60, 'D', "E\n\ncBeeAc\nBffBDFCDaacFba\0FBbefaaaeB\0A\ndd\n\0ECAddbcFbaFfEEb\nf", 59},
        {"FBbEDadddBddfdf\0Aa\nCCBbdCC\0FF\n\0BBDCeFBfBbCeAbECcA\nAeEeDbCBed", 60, 'c', "FBbEDadddBddfdf\0Aa\nCCBbdCC\0FF\n\0BBDCeFBfBbCeAbECA\nAeEeDbCBed", 59},
        {"Ff\0F\neCDAf\0DdcECEeddEFf\nEcBcAdef\naD\nCcdAEFBeA\0dFFaBd\0FFdCBD\n", 60, '\0', "FfF\neCDAf\0DdcECEeddEFf\nEcBcAdef\naD\nCcdAEFBeA\0dFFaBd\0FFdCBD\n", 59},
        {"dfBfbEEEefbFeEFfabde\nD\0ba\nFdEACdB\nE\nCaFbFcAAE\0Df\nCDFFaAeEAec", 60, 'E', "dfBfbEEefbFeEFfabde\nD\0ba\nFdEACdB\nE\nCaFbFcAAE\0Df\nCDFFaAeEAec", 59},
        {"BcD\0bDd\nDFf\0aeEDd\0CDdDdDCdf\0\n\0cCDF\nBBacCeCcCaDecfef\nccbdeBed", 60, 'F', "BcD\0bDd\nDf\0aeEDd\0CDdDdDCdf\0\n\0cCDF\nBBacCeCcCaDecfef\nccbdeBed", 59},
        {"\nFEbCedDAB\0b\nACDABEFcbBceBb\n\ncbdBfdbAb\nFDdfcfEaFdB\0\0BABffcac", 60, 'c', "\nFEbCedDAB\0b\nACDABEFbBceBb\n\ncbdBfdbAb\nFDdfcfEaFdB\0\0BABffcac", 59},
        {"Aa\nf\nFCAe\0BD\nbf\nFdA\0bBFD\nFbD\0dfC\0fbDcacbAfCa\0\nADBbfefFfEcAFc", 60, 'D', "Aa\nf\nFCAe\0B\nbf\nFdA\0bBFD\nFbD\0dfC\0fbDcacbAfCa\0\nADBbfefFfEcAFc", 59},
        {"B\nce\naCFca\nadEFbcdEEFBFf\ndCbbDdfA\0FDAAcAeabcBaaBEAB\0CEEeB\0\ne", 60, '\n', "Bce\naCFca\nadEFbcdEEFBFf\ndCbbDdfA\0FDAAcAeabcBaaBEAB\0CEEeB\0\ne", 59},
        {"EFCCbaf\0\0FedcFB\n\neEDaAFAC\0bFAaecA\n\naCCA\0EdcbCeb\ndBfaFeBBAa\n\0", 60, '\0', "EFCCbaf\0FedcFB\n\neEDaAFAC\0bFAaecA\n\naCCA\0EdcbCeb\ndBfaFeBBAa\n\0", 59},
        {"aCEcdbaDcfA\n\0CcfededbcFBAdcf\0caFCDb\ndff\n\nEAcD\0bdefFcBffEF\n\nE", 60, 'D', "aCEcdbacfA\n\0CcfededbcFBAdcf\0caFCDb\ndff\n\nEAcD\0bdefFcBffEF\n\nE", 59},
        {"aef\na\nC\0b\0AcabddbEFBD\0BDccBdDfadf\0eCDD\0acCeAf\0fBEdaAcfb\ncfCd", 60, 'a', "ef\na\nC\0b\0AcabddbEFBD\0BDccBdDfadf\0eCDD\0acCeAf\0fBEdaAcfb\ncfCd", 59},
        {"cceBaBEeDAde\nAcdC\nbaCE\nacbe\nb\0edFCE\nbBCFf\0AEFEC\nAfea\n\nD\nfbFB", 60, 'e', "ccBaBEeDAde\nAcdC\nbaCE\nacbe\nb\0edFCE\nbBCFf\0AEFEC\nAfea\n\nD\nfbFB", 59},
        {"ccFEDDcbedBcadbfeC\0\0\0\0ACaACeaedf\nfA\nbc\nFdE\n\ndDACEDaaA\nFCcEcd", 60, 'B', "ccFEDDcbedcadbfeC\0\0\0\0ACaACeaedf\nfA\nbc\nFdE\n\ndDACEDaaA\nFCcEcd", 59},
        {"DccAfCFadFEddfF\0c\nFEEfbCdDECBFCEab\0DFABaFd\nCf\nfEADfaFc\0AC\0Fd", 60, 'A', "DccfCFadFEddfF\0c\nFEEfbCdDECBFCEab\0DFABaFd\nCf\nfEADfaFc\0AC\0Fd", 59},
        {"dFeCadadbccBbbaC\0\nc\0\0DCceDfbAdfF\neEaffcbcebBebffFbBb\0fFDCdfF", 60, 'c', "dFeCadadbcBbbaC\0\nc\0\0DCceDfbAdfF\neEaffcbcebBebffFbBb\0fFDCdfF", 59},
        {"FbcEd\nfbfdFcABFDAfbbb\nEAeFceAB\0FFFfafffAe\nDeefFE\nCdeCbBFfE\nE", 60, 'c', "FbEd\nfbfdFcABFDAfbbb\nEAeFceAB\0FFFfafffAe\nDeefFE\nCdeCbBFfE\nE", 59},
        {"aabEcdFc\0Eba\nbEEbBfaEf\0AfefBcbeEdEaEEa\0CAAc\n\0aaabccCe\nbCFaDE", 60, 'a', "abEcdFc\0Eba\nbEEbBfaEf\0AfefBcbeEdEaEEa\0CAAc\n\0aaabccCe\nbCFaDE", 59},
        {"afeCc\nfbBEa\naA\0bF\0BCadcfbDcBd\n\n\0fccCFACbbACbAAEcafdAcd\0\nFCaA", 60, 'b', "afeCc\nfBEa\naA\0bF\0BCadcfbDcBd\n\n\0fccCFACbbACbAAEcafdAcd\0\nFCaA", 59},
        {"dbbF\nd\0B\0fACbfbaFCcCcd\nACFdedaccBcDcfadECEcfbFCDBCEfaBc\nAeCE", 60, 'e', "dbbF\nd\0B\0fACbfbaFCcCcd\nACFddaccBcDcfadECEcfbFCDBCEfaBc\nAeCE", 59},
        {"DffeFEbcD\nbeAAC\0deCbeaaFC\0DAACceBC\0fAbAbda\ncf\0\0Aafeb\0cBDCadd", 60, 'e', "DffFEbcD\nbeAAC\0deCbeaaFC\0DAACceBC\0fAbAbda\ncf\0\0Aafeb\0cBDCadd", 59},
        {"FcDFeDbcB\0EeFDEDeeDCcADcbBBFAafCefDAdADBb\0F\nEdA\naCB\n\n\ne\nCACC", 60, 'b', "FcDFeDcB\0EeFDEDeeDCcADcbBBFAafCefDAdADBb\0F\nEdA\naCB\n\n\ne\nCACC", 59},
        {"adcCf\nFEefedcaCDbFebdFcCEdEcD\nfcdA\nCfDAA\0ae\nd\0EefCEa\nFCfCBcd", 60, 'E', "adcCf\nFefedcaCDbFebdFcCEdEcD\nfcdA\nCfDAA\0ae\nd\0EefCEa\nFCfCBcd", 59},
        {"EbEacEbBDFD\0fCec\nCBaADCfeEcEcdfDeAe\0\0bfFEaCfAbBFFcbfeBFceCAA", 60, 'D', "EbEacEbBFD\0fCec\nCBaADCfeEcEcdfDeAe\0\0bfFEaCfAbBFFcbfeBFceCAA", 59},
        {"\0CB\nACFEDfe\nCcDcFEAfB\0dEFb\n\0ABFDFCCFaCcfFcaBadAabDecbDCbBebe", 60, 'd', "\0CB\nACFEDfe\nCcDcFEAfB\0EFb\n\0ABFDFCCFaCcfFcaBadAabDecbDCbBebe", 59},
        {"fcccdEaEECF\0\nAcC\nb\n\0dAbc\0Ca\nFC\0eAfb\ndAfaFc\0CfFBbCcb\nDFcBBdcB", 60, 'b', "fcccdEaEECF\0\nAcC\n\n\0dAbc\0Ca\nFC\0eAfb\ndAfaFc\0CfFBbCcb\nDFcBBdcB", 59},
        {"bC\0eBDbFdCddEDf\n\0EA\naFAB\n\0fEFcfBDbfcbFe\0fcf\0ffBAd\0\0af\n\nC\nFF\n", 60, 'E', "bC\0eBDbFdCddDf\n\0EA\naFAB\n\0fEFcfBDbfcbFe\0fcf\0ffBAd\0\0af\n\nC\nFF\n", 59},
        {"DBe\0AbDaA\0cAb\0DaddbB\nCEFBCCaCceAccA\ncdCcacDE\neaAbdFCc\0DdcCFa", 60, 'b', "DBe\0ADaA\0cAb\0DaddbB\nCEFBCCaCceAccA\ncdCcacDE\neaAbdFCc\0DdcCFa", 59},
        {"d\0\nBB\0eC\nEeFBfae\0EFCfBDafCBe\ncFaC\ndFE\0BdAeCeFFFfaCccEDfeaEeF", 60, 'b', "d\0\nBB\0eC\nEeFBfae\0EFCfBDafCBe\ncFaC\ndFE\0BdAeCeFFFfaCccEDfeaEeF", 60},
        {"EACCcecAFBc\0\nEdBcdE\n\0e\0EFe\0A\0bBfFcCCDef\ndCfAbeebDfB\0eFcafcEd", 60, 'c', "EACCecAFBc\0\nEdBcdE\n\0e\0EFe\0A\0bBfFcCCDef\ndCfAbeebDfB\0eFcafcEd", 59},
        {"", 0, 'a', "", 0}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeFirstChar(str, test_cases[i].sub);

        assert(str->len == test_cases[i].expected_len && "xString_removeFirstChar() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeFirstChar() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}
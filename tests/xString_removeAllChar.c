// unit test for xString_removeAllChar function in xString module

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
        {"fbCEBebf\0FaBedBaDAfaadAe\0\0FecCBbEdDbbFbaCfCfdF\0\nbd\nDb\nbcaECd", 60, 'D', "fbCEBebf\0FaBedBaAfaadAe\0\0FecCBbEdbbFbaCfCfdF\0\nbd\nb\nbcaECd", 57},
        {"E\n\ncBeeADc\nBffBDFCDaacFba\0FBbefaaaeB\0A\ndd\n\0ECAddbcFbaFfEEb\nf", 60, 'D', "E\n\ncBeeAc\nBffBFCaacFba\0FBbefaaaeB\0A\ndd\n\0ECAddbcFbaFfEEb\nf", 57},
        {"FBbEDadddBddfdf\0Aa\nCCBbdCC\0FF\n\0BBDCeFBfBbCeAbECcA\nAeEeDbCBed", 60, 'c', "FBbEDadddBddfdf\0Aa\nCCBbdCC\0FF\n\0BBDCeFBfBbCeAbECA\nAeEeDbCBed", 59},
        {"Ff\0F\neCDAf\0DdcECEeddEFf\nEcBcAdef\naD\nCcdAEFBeA\0dFFaBd\0FFdCBD\n", 60, '\0', "FfF\neCDAfDdcECEeddEFf\nEcBcAdef\naD\nCcdAEFBeAdFFaBdFFdCBD\n", 56},
        {"dfBfbEEEefbFeEFfabde\nD\0ba\nFdEACdB\nE\nCaFbFcAAE\0Df\nCDFFaAeEAec", 60, 'E', "dfBfbefbFeFfabde\nD\0ba\nFdACdB\n\nCaFbFcAA\0Df\nCDFFaAeAec", 52},
        {"BcD\0bDd\nDFf\0aeEDd\0CDdDdDCdf\0\n\0cCDF\nBBacCeCcCaDecfef\nccbdeBed", 60, 'F', "BcD\0bDd\nDf\0aeEDd\0CDdDdDCdf\0\n\0cCD\nBBacCeCcCaDecfef\nccbdeBed", 58},
        {"\nFEbCedDAB\0b\nACDABEFcbBceBb\n\ncbdBfdbAb\nFDdfcfEaFdB\0\0BABffcac", 60, 'c', "\nFEbCedDAB\0b\nACDABEFbBeBb\n\nbdBfdbAb\nFDdffEaFdB\0\0BABffa", 54},
        {"Aa\nf\nFCAe\0BD\nbf\nFdA\0bBFD\nFbD\0dfC\0fbDcacbAfCa\0\nADBbfefFfEcAFc", 60, 'D', "Aa\nf\nFCAe\0B\nbf\nFdA\0bBF\nFb\0dfC\0fbcacbAfCa\0\nABbfefFfEcAFc", 55},
        {"B\nce\naCFca\nadEFbcdEEFBFf\ndCbbDdfA\0FDAAcAeabcBaaBEAB\0CEEeB\0\ne", 60, '\n', "BceaCFcaadEFbcdEEFBFfdCbbDdfA\0FDAAcAeabcBaaBEAB\0CEEeB\0e", 55},
        {"EFCCbaf\0\0FedcFB\n\neEDaAFAC\0bFAaecA\n\naCCA\0EdcbCeb\ndBfaFeBBAa\n\0", 60, '\0', "EFCCbafFedcFB\n\neEDaAFACbFAaecA\n\naCCAEdcbCeb\ndBfaFeBBAa\n", 55},
        {"aCEcdbaDcfA\n\0CcfededbcFBAdcf\0caFCDb\ndff\n\nEAcD\0bdefFcBffEF\n\nE", 60, 'D', "aCEcdbacfA\n\0CcfededbcFBAdcf\0caFCb\ndff\n\nEAc\0bdefFcBffEF\n\nE", 57},
        {"aef\na\nC\0b\0AcabddbEFBD\0BDccBdDfadf\0eCDD\0acCeAf\0fBEdaAcfb\ncfCd", 60, 'a', "ef\n\nC\0b\0AcbddbEFBD\0BDccBdDfdf\0eCDD\0cCeAf\0fBEdAcfb\ncfCd", 54},
        {"cceBaBEeDAde\nAcdC\nbaCE\nacbe\nb\0edFCE\nbBCFf\0AEFEC\nAfea\n\nD\nfbFB", 60, 'e', "ccBaBEDAd\nAcdC\nbaCE\nacb\nb\0dFCE\nbBCFf\0AEFEC\nAfa\n\nD\nfbFB", 54},
        {"ccFEDDcbedBcadbfeC\0\0\0\0ACaACeaedf\nfA\nbc\nFdE\n\ndDACEDaaA\nFCcEcd", 60, 'B', "ccFEDDcbedcadbfeC\0\0\0\0ACaACeaedf\nfA\nbc\nFdE\n\ndDACEDaaA\nFCcEcd", 59},
        {"DccAfCFadFEddfF\0c\nFEEfbCdDECBFCEab\0DFABaFd\nCf\nfEADfaFc\0AC\0Fd", 60, 'A', "DccfCFadFEddfF\0c\nFEEfbCdDECBFCEab\0DFBaFd\nCf\nfEDfaFc\0C\0Fd", 56},
        {"dFeCadadbccBbbaC\0\nc\0\0DCceDfbAdfF\neEaffcbcebBebffFbBb\0fFDCdfF", 60, 'c', "dFeCadadbBbbaC\0\n\0\0DCeDfbAdfF\neEaffbebBebffFbBb\0fFDCdfF", 54},
        {"FbcEd\nfbfdFcABFDAfbbb\nEAeFceAB\0FFFfafffAe\nDeefFE\nCdeCbBFfE\nE", 60, 'c', "FbEd\nfbfdFABFDAfbbb\nEAeFeAB\0FFFfafffAe\nDeefFE\nCdeCbBFfE\nE", 57},
        {"aabEcdFc\0Eba\nbEEbBfaEf\0AfefBcbeEdEaEEa\0CAAc\n\0aaabccCe\nbCFaDE", 60, 'a', "bEcdFc\0Eb\nbEEbBfEf\0AfefBcbeEdEEE\0CAAc\n\0bccCe\nbCFDE", 50},
        {"afeCc\nfbBEa\naA\0bF\0BCadcfbDcBd\n\n\0fccCFACbbACbAAEcafdAcd\0\nFCaA", 60, 'b', "afeCc\nfBEa\naA\0F\0BCadcfDcBd\n\n\0fccCFACACAAEcafdAcd\0\nFCaA", 54},
        {"dbbF\nd\0B\0fACbfbaFCcCcd\nACFdedaccBcDcfadECEcfbFCDBCEfaBc\nAeCE", 60, 'e', "dbbF\nd\0B\0fACbfbaFCcCcd\nACFddaccBcDcfadECEcfbFCDBCEfaBc\nACE", 58},
        {"DffeFEbcD\nbeAAC\0deCbeaaFC\0DAACceBC\0fAbAbda\ncf\0\0Aafeb\0cBDCadd", 60, 'e', "DffFEbcD\nbAAC\0dCbaaFC\0DAACcBC\0fAbAbda\ncf\0\0Aafb\0cBDCadd", 54},
        {"FcDFeDbcB\0EeFDEDeeDCcADcbBBFAafCefDAdADBb\0F\nEdA\naCB\n\n\ne\nCACC", 60, 'b', "FcDFeDcB\0EeFDEDeeDCcADcBBFAafCefDAdADB\0F\nEdA\naCB\n\n\ne\nCACC", 57},
        {"adcCf\nFEefedcaCDbFebdFcCEdEcD\nfcdA\nCfDAA\0ae\nd\0EefCEa\nFCfCBcd", 60, 'E', "adcCf\nFefedcaCDbFebdFcCdcD\nfcdA\nCfDAA\0ae\nd\0efCa\nFCfCBcd", 55},
        {"EbEacEbBDFD\0fCec\nCBaADCfeEcEcdfDeAe\0\0bfFEaCfAbBFFcbfeBFceCAA", 60, 'D', "EbEacEbBF\0fCec\nCBaACfeEcEcdfeAe\0\0bfFEaCfAbBFFcbfeBFceCAA", 56},
        {"\0CB\nACFEDfe\nCcDcFEAfB\0dEFb\n\0ABFDFCCFaCcfFcaBadAabDecbDCbBebe", 60, 'd', "\0CB\nACFEDfe\nCcDcFEAfB\0EFb\n\0ABFDFCCFaCcfFcaBaAabDecbDCbBebe", 58},
        {"fcccdEaEECF\0\nAcC\nb\n\0dAbc\0Ca\nFC\0eAfb\ndAfaFc\0CfFBbCcb\nDFcBBdcB", 60, 'b', "fcccdEaEECF\0\nAcC\n\n\0dAc\0Ca\nFC\0eAf\ndAfaFc\0CfFBCc\nDFcBBdcB", 55},
        {"bC\0eBDbFdCddEDf\n\0EA\naFAB\n\0fEFcfBDbfcbFe\0fcf\0ffBAd\0\0af\n\nC\nFF\n", 60, 'E', "bC\0eBDbFdCddDf\n\0A\naFAB\n\0fFcfBDbfcbFe\0fcf\0ffBAd\0\0af\n\nC\nFF\n", 57},
        {"DBe\0AbDaA\0cAb\0DaddbB\nCEFBCCaCceAccA\ncdCcacDE\neaAbdFCc\0DdcCFa", 60, 'b', "DBe\0ADaA\0cA\0DaddB\nCEFBCCaCceAccA\ncdCcacDE\neaAdFCc\0DdcCFa", 56},
        {"d\0\nBB\0eC\nEeFBfae\0EFCfBDafCBe\ncFaC\ndFE\0BdAeCeFFFfaCccEDfeaEeF", 60, 'b', "d\0\nBB\0eC\nEeFBfae\0EFCfBDafCBe\ncFaC\ndFE\0BdAeCeFFFfaCccEDfeaEeF", 60},
        {"EACCcecAFBc\0\nEdBcdE\n\0e\0EFe\0A\0bBfFcCCDef\ndCfAbeebDfB\0eFcafcEd", 60, 'c', "EACCeAFB\0\nEdBdE\n\0e\0EFe\0A\0bBfFCCDef\ndCfAbeebDfB\0eFafEd", 53},
        {"", 0, 'a', "", 0}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllChar(str, test_cases[i].sub);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllChar() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllChar() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}
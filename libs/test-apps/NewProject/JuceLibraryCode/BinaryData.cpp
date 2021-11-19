/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== fast_ptm.txt ==================
static const unsigned char temp_binary_data_0[] =
"Ideas for accelerating PTM computation\n"
"--------------------------------------\n"
"\n"
"First thing to note is that codebook computation now takes up the\n"
"majority of the time spent evaluating PTMs.  So speeding up Gaussian\n"
"evaluation is suddenly important again.\n"
"\n"
"Using a tighter top-N beam will speed up Gaussian computation by\n"
"imposing a higher floor on densities, but this effect isn't worth a\n"
"whole lot, in contrast to SC models where mixture computation rather\n"
"than density computation is the most expensive part.\n"
"\n"
"This means that we should probably bring back kd-trees, although the\n"
"implementation should be tweaked to be faster loading.\n"
"\n"
"Also, maybe more importantly, we can do some form of CI-GMM selection\n"
"on the codebooks.  This won't actually work with the way the models\n"
"are set up currently since the CI phones share the same codebook as\n"
"the CD ones, and the goal is to prune codebooks rather than phones.\n"
"\n"
"But wait!  It's okay, because we still have the same top-N mechanism\n"
"as before.  We can use those top-N scores to do early pruning of\n"
"entire codebooks.  This ought to give us the most bang for the buck.\n";

const char* fast_ptm_txt = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xe4d86b37:  numBytes = 1114; return fast_ptm_txt;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "fast_ptm_txt"
};

const char* originalFilenames[] =
{
    "fast_ptm.txt"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}

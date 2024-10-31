#ifndef ImageModel_hpp
#define ImageModel_hpp

/**
 * @brief 
 * 
*/
class ImageModel
{
    public:
        /**
         * @brief 
        */
        ImageModel(int iModelWidth, int iModelHeight);
        
        void Resize();
        void Padding();

    protected:
        int iModelWidth;
        int iModelHeight;

};

#endif // ImageModel_hpp
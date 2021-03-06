/*IlwisObjects is a framework for analysis, processing and visualization of remote sensing and gis data
Copyright (C) 2018  52n North

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#pragma once

#include "raster.h"
#include <vector>

namespace Ilwis {

	namespace Ui {
        class RasterLayerModel;
        class Texture;
        class TextureHeap
	    {
	    public:
			const int TEXAVAILABLE = -1;
			const int TEXNONE = -2;
            friend class TextureGenerator;
			TextureHeap(RasterLayerModel * rasterLayerModel, const std::vector<IRasterCoverage> * ccBands);
		    TextureHeap(RasterLayerModel * rasterLayerModel, const IRasterCoverage & raster, const unsigned int iPaletteSize, bool asAnimation);
		    virtual ~TextureHeap();

		    Texture * GetTexture(int bandIndex,bool & optimal, const unsigned int offsetX, const unsigned int offsetY, const unsigned int sizeX, const unsigned int sizeY, unsigned int zoomFactor, bool fInThread); // Pre: optimal = false; Post; optimal = true if and only if the texture was an exact match with no dirty flag
		    void ClearQueuedTextures();
		    void ReGenerateAllTextures();
		    bool renderingComplete();
            bool findTexture(int bandIndex, const unsigned int offsetX, const unsigned int offsetY, const unsigned int sizeX, const unsigned int sizeY, unsigned int zoomFactor);
			int hasOptimalTexture(int bandIndex, const unsigned int offsetX, const unsigned int offsetY, const unsigned int sizeX, const unsigned int sizeY, unsigned int zoomFactor) const;
			Texture * GenerateTexture(int bandIndex, const unsigned int offsetX, const unsigned int offsetY, const unsigned int sizeX, const unsigned int sizeY, unsigned int zoomFactor, bool fInThread);

	    protected:


		    void ReGenerateTexture(Texture * texture, bool fInThread);
		    Texture * GenerateNextTexture(bool fInThread);
		    std::vector<std::vector<Texture*>> textures;
		    std::vector<Texture*> textureRequest;
		    IRasterCoverage raster;
			const std::vector<IRasterCoverage> * _ccBands = 0;
		    unsigned int iPaletteSize;
		    bool fAbortTexGen;
		    TextureGenerator * textureThread;
		    bool fStopThread;
            QMutex csChangeTexCreatorList;
            QMutex csThread;
            QWaitCondition pauseThread;
		    Texture * workingTexture;
            RasterLayerModel * rasterLayerModel;
            bool fColorComposite;
	    };

        class TextureGenerator : public QThread
        {
            Q_OBJECT
        public:
            TextureGenerator(TextureHeap * heap);
            void run() Q_DECL_OVERRIDE;

        signals:
            void requestRedraw();

        private:
            TextureHeap * pObject;
        };
	}
}

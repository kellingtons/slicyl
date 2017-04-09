/***************************************************************************
The MIT License

Copyright (c) 2017 Kyle Ruan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "SlicedLayers.h"

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Class constructor.
--| Args:
--|     none
--| Return:
--|     A SlicedLayers Object
--|-------------------------------------------------------------------------
*/
SlicedLayers::SlicedLayers(void)
{

}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Class destructor.
--| Args:
--|     None
--| Return:
--|     None
--|-------------------------------------------------------------------------
*/
SlicedLayers::~SlicedLayers(void)
{

}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Gets a handle on all the sliced layers
--| Args:
--|     None
--| Return:
--|     A vector of vectors of slicepieces
--|-------------------------------------------------------------------------
*/
std::vector<std::vector<slicepiece> > SlicedLayers::GetSlicedLayers()
{
    return all_layers;
}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Adds a new outer layer for one slice
--| Args:
--|     None
--| Return:
--|     None
--|-------------------------------------------------------------------------
*/
void SlicedLayers::AddLayer(std::vector<slicepiece> &layer) 
{
    all_layers.push_back(layer);
}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Removes the outer layer for one slice
--| Args:
--|     None
--| Return:
--|     None
--|-------------------------------------------------------------------------
*/
void SlicedLayers::RemovePiece()
{
    all_layers.pop_back();
}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Get how many layers there are 
--| Args:
--|     none
--| Return:
--|     size_t - How many layers there are
--|-------------------------------------------------------------------------
*/
size_t SlicedLayers::GetSize() const
{
    return all_layers.size();
}

/*
--|-------------------------------------------------------------------------
--| Purpose:
--|     Gets a specific layer
--| Args:
--|     i - Which layer is being requested
--| Return:
--|     const vector containing all the slice pieces at the requested slice
--|-------------------------------------------------------------------------
*/
const std::vector<slicepiece> SlicedLayers::GetLayer(int i)
{
    return all_layers[i];
}
/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2019-2023 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::breadOvenSidesFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition provides a base class for 'breadOvenSides' type boundary
    conditions, i.e. conditions that mix fixed value and patch-normal gradient
    conditions.

    Usage:

    type breadOvenSides;
    alpha           10; // -- heat transfer coefficient
    TInf            463;    // -- oven temperature

    // -- mixed BC mandatory entires
    refValue        uniform 303;
    refGradient     uniform 0;
    valueFraction   uniform 0;
    value           uniform 303;

Note
    Boundary condition for temperature field for bread in oven.

SourceFiles
    breadOvenSidesFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef breadOvenSides_H
#define breadOvenSides_H

#include "mixedFvPatchFields.H"

namespace Foam
{

class breadOvenSidesFvPatchScalarField
:
    public mixedFvPatchScalarField
{
public:
    scalar alpha_;
    TypeName("breadOvenSides");

    breadOvenSidesFvPatchScalarField(const fvPatch&, const DimensionedField<scalar, volMesh>&);
    breadOvenSidesFvPatchScalarField(const fvPatch&, const DimensionedField<scalar, volMesh>&, const dictionary&);
    breadOvenSidesFvPatchScalarField(const breadOvenSidesFvPatchScalarField&, const fvPatch&, const DimensionedField<scalar, volMesh>&, const fvPatchFieldMapper&);
    breadOvenSidesFvPatchScalarField(const breadOvenSidesFvPatchScalarField&);
    virtual void write(Ostream& os) const;
    virtual void updateCoeffs();
};

}

#endif
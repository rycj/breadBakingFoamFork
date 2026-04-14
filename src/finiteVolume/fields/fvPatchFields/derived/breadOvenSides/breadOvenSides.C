/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2023 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "breadOvenSides.H"
#include "fvPatchFields.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "addToRunTimeSelectionTable.H"

namespace Foam
{
breadOvenSidesFvPatchScalarField::breadOvenSidesFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF)
{}

breadOvenSidesFvPatchScalarField::breadOvenSidesFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF, dict),
    alpha_(dict.get<scalar>("alpha_"))
{}



void Foam::breadOvenSidesFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    if(this->db().objectRegistry::foundObject<volScalarField>("nut"))
    {
        scalarField nut = this->db().lookupObject<volScalarField>("nut").boundaryField()[this->patch().index()];
        scalarField rho = this->db().lookupObject<volScalarField>("rho").boundaryField()[this->patch().index()];
        if (nut.size() != 0 && rho.size() != 0)
        {

            IOdictionary thermophysicalProperties = this->db().objectRegistry::lookupObject<IOdictionary>("thermophysicalProperties");
            scalar Pr;
            thermophysicalProperties.subDict("mixture").subDict("transport").readEntry("Pr",Pr);
            scalar mu;
            thermophysicalProperties.subDict("mixture").subDict("transport").readEntry("mu",mu);
            scalar cp;
            thermophysicalProperties.subDict("mixture").subDict("thermodynamics").readEntry("Cp",cp);
            // scalarField nut = this->db().lookupObject<volScalarField>("nut").boundaryField()[this->patch().index()];
            // scalarField rho = this->db().lookupObject<volScalarField>("rho").boundaryField()[this->patch().index()];

            scalarField lambdaEffBound = ((mu+nut*rho)*cp/Pr);

            Info <<"min(lambdaEffBound)"<< min(lambdaEffBound) << endl;

            // scalarField lambdaEffBound = lambdaEff.boundaryField()[this->patch().index()];
            // scalarField f = 1.0 / (1.0 + (lambdaEffBound / (mag(this->patch().delta() + (DBound - DCells)))) / (alpha_));
            scalarField f = 1.0 / (1.0 + (lambdaEffBound * this->patch().deltaCoeffs()) / (alpha_));
            this->valueFraction() = f;
            this->refValue() = 300;
        }
    }
    mixedFvPatchScalarField::updateCoeffs();
           
}

void Foam::breadOvenSidesFvPatchScalarField::write(Ostream& os) const
{
    mixedFvPatchScalarField::write(os);

    os.writeEntry("alpha_", alpha_);

    writeEntry("value",os);
}

breadOvenSidesFvPatchScalarField::breadOvenSidesFvPatchScalarField
(
    const breadOvenSidesFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper)
{}
}
#include "addToRunTimeSelectionTable.H"

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        breadOvenSidesFvPatchScalarField
    );
}
// ************************************************************************* //

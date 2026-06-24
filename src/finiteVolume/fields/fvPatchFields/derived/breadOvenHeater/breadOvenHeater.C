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

#include "breadOvenHeater.H"
#include "fvPatchFields.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "addToRunTimeSelectionTable.H"
// #include "rhoThermo.H"

namespace Foam
{
breadOvenHeaterFvPatchScalarField::breadOvenHeaterFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    currentTime_(-1)
{}

breadOvenHeaterFvPatchScalarField::breadOvenHeaterFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF, dict),
    q_(dict.get<scalar>("q_")),
    TStop_(dict.get<scalar>("TStop_")),
    TStart_(dict.get<scalar>("TStart_")),
    stopped(false),
    currentTime_(-1)
{}



void Foam::breadOvenHeaterFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }
    if (db().time().value() == currentTime_)
    {
        return;
    }
    currentTime_ = db().time().value();

            scalarField nut = this->db().lookupObject<volScalarField>("nut").boundaryField()[this->patch().index()];
            scalarField rho = this->db().lookupObject<volScalarField>("rho").boundaryField()[this->patch().index()];
            scalarField alpha = this->db().lookupObject<volScalarField>("thermo:alpha").boundaryField()[this->patch().index()];
            scalarField alphat = this->db().lookupObject<volScalarField>("alphat").boundaryField()[this->patch().index()];
            
            // scalarField alphaEff = alpha;
            

            // const rhoThermo& thermo =this->db().lookupObject<rhoThermo>("thermophysicalProperties");
            // const volScalarField& he =this->db().lookupObject<volScalarField>("h");

            // const scalarField& hePatch =he.boundaryField()[patch().index()];

            // tmp<scalarField> tAlphaEff =thermo.alphaEff(hePatch, patch().index());

            // const scalarField& alphaEff = tAlphaEff();
            if (nut.size() != 0 && rho.size() != 0)
            {
            const volScalarField& T = this->db().objectRegistry::lookupObject<volScalarField>("T");
            IOdictionary thermophysicalProperties = this->db().objectRegistry::lookupObject<IOdictionary>("thermophysicalProperties");
            scalar Pr;
            thermophysicalProperties.subDict("mixture").subDict("transport").readEntry("Pr",Pr);
            scalar mu;
            thermophysicalProperties.subDict("mixture").subDict("transport").readEntry("mu",mu);
            scalar cp;
            thermophysicalProperties.subDict("mixture").subDict("thermodynamics").readEntry("Cp",cp);
            scalar Tp;
            thermophysicalProperties.subDict("mixture").subDict("transport").readEntry("mu",mu);

            scalarField alphaEff = alpha+alphat;

            const fvMesh& mesh = this->patch().boundaryMesh().mesh();
            const surfaceVectorField& Sf = mesh.Sf();
            vectorField SfBound = Sf.boundaryField()[this->patch().index()];
            scalarField SfBoundSize =mag(SfBound);

            scalar probeT = Foam::max(T.internalField()).value();
            scalar q;

            if(probeT<TStart_)
            {
                q=q_;
                stopped=false;
            }
            else if(probeT<TStop_&& !stopped)
            {
                q=q_;
            }
            else if(probeT>TStop_)
            {
                q=0;
                stopped=true;
            }
            else
            {
                q=0;
            }

 
            // this->refGrad()=(q/((mu+nut*rho)*cp/Pr));
            this->refGrad()=(q/(alphaEff*cp));
            // this->refGrad()=(q/gSum(patch().magSf()) + qr)/kappa(Tp);
            this->valueFraction() = 0.0;
            }
    mixedFvPatchScalarField::updateCoeffs();
           
}

void Foam::breadOvenHeaterFvPatchScalarField::write(Ostream& os) const
{
    mixedFvPatchScalarField::write(os);

    os.writeEntry("q_", q_);
    os.writeEntry("TStop_", TStop_);
    os.writeEntry("TStart_", TStart_);

    writeEntry("value", os);
}

breadOvenHeaterFvPatchScalarField::breadOvenHeaterFvPatchScalarField
(
    const breadOvenHeaterFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper),
    currentTime_(ptf.currentTime_)
{}
}
#include "addToRunTimeSelectionTable.H"

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        breadOvenHeaterFvPatchScalarField
    );
}
// ************************************************************************* //

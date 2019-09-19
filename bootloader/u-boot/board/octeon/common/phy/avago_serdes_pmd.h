/* **************************************************************** */
/*                                                                  */
/* ASIC and ASSP Programming Layer (AAPL)                           */
/* (C) Copyright 2014-2015 Avago Technologies. All rights reserved. */
/*                                                                  */
/* **************************************************************** *
 * This file is part of the AAPL CORE library.
 *
 * AAPL CORE is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AAPL CORE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AAPL CORE.  If not, see http://www.gnu.org/licenses.
 *
 * AAPL Revision: 2.2.3
 */
/* AAPL (ASIC and ASSP Programming Layer) generic handling of SerDes */
/* (Serializer/Deserializer) slices on ASIC SBus rings; all functions exported */
/* from this directory. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for SerDes PMD/KR functions. */

#ifndef AVAGO_SERDES_PMD_H_
#define AVAGO_SERDES_PMD_H_

/*////////////////////////////////////////////////////////////////////////////// */
/* START: KR functions and structs */

/**@brief Controls which Rx DFE settings are updated. */
enum avago_serdes_pmd_train_mode {
	AVAGO_PMD_RESTART,        /** Reset the PMD training */
	AVAGO_PMD_BYPASS,         /** Set Tx Eq to Initialize and start sending core data */
	AVAGO_PMD_TRAIN           /** Run full PMD training */
};

/**@brief Controls which Rx DFE settings are updated. */
enum avago_serdes_pmd_clause {
	AVAGO_PMD_FC16G,         /** PRBS11 - x^11 + x^9 + 1 */
	AVAGO_PMD_CL72,          /** PRBS11 - x^11 + x^9 + 1 */
	AVAGO_PMD_CL84,          /** PRBS11 - x^11 + x^9 + 1 */
	AVAGO_PMD_CL92           /** PRBS11 - sequence based on lane config */
};

#endif /* AVAGO_SERDES_PMD_H_ */

--
-- PostgreSQL database dump
--

-- Dumped from database version 9.6.3
-- Dumped by pg_dump version 10.13

-- Started on 2023-05-09 21:52:43

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 1 (class 3079 OID 12391)
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- TOC entry 2487 (class 0 OID 0)
-- Dependencies: 1
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 185 (class 1259 OID 17081)
-- Name: agents; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.agents (
    agentid integer NOT NULL,
    agentname character varying(64) NOT NULL,
    agentcode character varying(8) NOT NULL,
    plate character varying(20),
    farm integer NOT NULL,
    mig_id integer
);


ALTER TABLE public.agents OWNER TO postgres;

--
-- TOC entry 186 (class 1259 OID 17084)
-- Name: agents_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.agents_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.agents_seq OWNER TO postgres;

--
-- TOC entry 187 (class 1259 OID 17086)
-- Name: invoicedeductions_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.invoicedeductions_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.invoicedeductions_seq OWNER TO postgres;

--
-- TOC entry 188 (class 1259 OID 17088)
-- Name: buy_invoice_deductions; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.buy_invoice_deductions (
    season integer NOT NULL,
    invoice integer NOT NULL,
    code character varying(10) NOT NULL,
    fraction numeric(15,5) NOT NULL,
    reason character varying(300) NOT NULL,
    enabled integer NOT NULL,
    id integer DEFAULT nextval('public.invoicedeductions_seq'::regclass) NOT NULL
);


ALTER TABLE public.buy_invoice_deductions OWNER TO postgres;

--
-- TOC entry 189 (class 1259 OID 17092)
-- Name: buy_invoice_entries; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.buy_invoice_entries (
    cow integer NOT NULL,
    invoice integer NOT NULL,
    price numeric(15,5) NOT NULL,
    class integer NOT NULL,
    weight numeric(15,5) NOT NULL,
    season integer NOT NULL
);


ALTER TABLE public.buy_invoice_entries OWNER TO postgres;

--
-- TOC entry 190 (class 1259 OID 17095)
-- Name: buy_invoice_hents; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.buy_invoice_hents (
    name character varying(150) NOT NULL,
    zip character varying(10),
    city character varying(48),
    street character varying(48),
    pobox character varying(10),
    fiscal_no character(10),
    accountno character(28),
    bankname character varying(50),
    personal_no character(11),
    personal_id_no character varying(12),
    issuepost character varying(50),
    issuedate date,
    season integer NOT NULL,
    parent_hent integer NOT NULL,
    invoice integer NOT NULL,
    hentno character(14) NOT NULL
);


ALTER TABLE public.buy_invoice_hents OWNER TO postgres;

--
-- TOC entry 191 (class 1259 OID 17098)
-- Name: buy_invoices; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.buy_invoices (
    invoiceno integer NOT NULL,
    invoiceid integer NOT NULL,
    extras character varying(255),
    invoicedate date NOT NULL,
    paid date,
    invoice_type character(1) NOT NULL,
    vatrate numeric(15,5) NOT NULL,
    payway character(1) NOT NULL,
    payduedays integer NOT NULL,
    transactionplace character varying(48),
    transactiondate date,
    season integer NOT NULL,
    customnumber character varying(15),
    invoicescope character(1) NOT NULL,
    correct_for integer,
    mig_id integer,
    CONSTRAINT buy_invoices_invoice_type_check CHECK (((invoice_type = 'L'::bpchar) OR (invoice_type = 'V'::bpchar))),
    CONSTRAINT buy_invoices_invoicescope_check CHECK (((invoicescope = 'C'::bpchar) OR (invoicescope = 'D'::bpchar))),
    CONSTRAINT buy_invoices_payduedays_check CHECK (((payduedays >= 0) AND (payduedays <= 365))),
    CONSTRAINT buy_invoices_payway_check CHECK (((payway = '$'::bpchar) OR (payway = '@'::bpchar)))
);


ALTER TABLE public.buy_invoices OWNER TO postgres;

--
-- TOC entry 192 (class 1259 OID 17105)
-- Name: buydoc_cows; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.buydoc_cows (
    cow integer NOT NULL,
    doc integer NOT NULL,
    "group" smallint,
    stock integer,
    weight numeric(15,5),
    class integer,
    season integer NOT NULL
);


ALTER TABLE public.buydoc_cows OWNER TO postgres;

--
-- TOC entry 193 (class 1259 OID 17108)
-- Name: buydocs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.buydocs (
    season integer NOT NULL,
    herd integer NOT NULL,
    hent integer NOT NULL,
    docdate date NOT NULL,
    extras character varying(255),
    plateno character varying(20),
    loaddate date NOT NULL,
    docid integer NOT NULL,
    invoice integer,
    loadstrtm integer,
    loadendtm integer,
    agent integer,
    motive character(1) NOT NULL,
    mig_id integer,
    CONSTRAINT buydocs_motive_check CHECK (((motive = 'B'::bpchar) OR (motive = '+'::bpchar) OR (motive = 'I'::bpchar)))
);


ALTER TABLE public.buydocs OWNER TO postgres;

--
-- TOC entry 194 (class 1259 OID 17112)
-- Name: buydocs_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.buydocs_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.buydocs_seq OWNER TO postgres;

--
-- TOC entry 195 (class 1259 OID 17114)
-- Name: buyinvoices_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.buyinvoices_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.buyinvoices_seq OWNER TO postgres;

--
-- TOC entry 196 (class 1259 OID 17116)
-- Name: cattle; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.cattle (
    animalid integer NOT NULL,
    cowno character varying(14) NOT NULL,
    birthdate date,
    birthplace character varying(32),
    sex smallint,
    stock integer,
    motherno character varying(14),
    weight numeric(15,5) NOT NULL,
    passno character(10),
    passdate date,
    fstownr integer,
    extras character varying(255),
    termbuyclass integer,
    termbuyweight numeric(15,5),
    termbuyprice numeric(15,5),
    termsellclass integer,
    termsellweight numeric(15,5),
    termsellprice numeric(15,5),
    buy_invoice integer,
    sell_invoice integer,
    passimglabel character(24),
    healthcertno character varying(16),
    class integer NOT NULL,
    season integer NOT NULL,
    mig_id integer,
    CONSTRAINT cattle_sex_check CHECK (((sex = '-1'::integer) OR (sex = 1) OR (sex = 0)))
);


ALTER TABLE public.cattle OWNER TO postgres;

--
-- TOC entry 197 (class 1259 OID 17120)
-- Name: cattle_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.cattle_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.cattle_seq OWNER TO postgres;

--
-- TOC entry 198 (class 1259 OID 17122)
-- Name: classes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.classes (
    classname character varying(64),
    classcode character varying(8) NOT NULL,
    class_id integer NOT NULL,
    predefsex integer,
    myperkgprice numeric(15,5),
    farm integer NOT NULL,
    mig_id integer,
    CONSTRAINT classes_predefsex_check CHECK (((predefsex = '-1'::integer) OR (predefsex = 1) OR (predefsex = 0)))
);


ALTER TABLE public.classes OWNER TO postgres;

--
-- TOC entry 199 (class 1259 OID 17126)
-- Name: classes_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.classes_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.classes_seq OWNER TO postgres;

--
-- TOC entry 200 (class 1259 OID 17128)
-- Name: counters; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.counters (
    name character varying(30) NOT NULL,
    value integer NOT NULL,
    season integer NOT NULL
);


ALTER TABLE public.counters OWNER TO postgres;

--
-- TOC entry 201 (class 1259 OID 17131)
-- Name: deductiondefinitions_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.deductiondefinitions_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.deductiondefinitions_seq OWNER TO postgres;

--
-- TOC entry 202 (class 1259 OID 17133)
-- Name: farm_bank_accounts; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.farm_bank_accounts (
    farm integer NOT NULL,
    alias character varying(20) NOT NULL,
    accoutno character varying(28) NOT NULL,
    bank_name character varying(60),
    account_id integer NOT NULL
);


ALTER TABLE public.farm_bank_accounts OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 17136)
-- Name: farms; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.farms (
    farm_id integer NOT NULL,
    name character varying(120) NOT NULL,
    zip character varying(10) NOT NULL,
    city character varying(48) NOT NULL,
    street character varying(48) NOT NULL,
    pobox character varying(10) NOT NULL,
    fiscal_no character varying(10) NOT NULL,
    farmno character varying(11) NOT NULL,
    wetidno character varying(8),
    phone character varying(12),
    accountno character(28),
    bankname character varying(50),
    stats_no character(9),
    wetlicenceno character varying(12),
    alias character varying(50)
);


ALTER TABLE public.farms OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 17139)
-- Name: hents; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.hents (
    hentid integer NOT NULL,
    alias character varying(24) NOT NULL,
    name character varying(150) NOT NULL,
    zip character varying(10),
    city character varying(48),
    street character varying(48),
    pobox character varying(10),
    fiscal_no character(10),
    hentno character(14) NOT NULL,
    wetno character(8),
    phone_no character varying(12),
    plate_no character varying(16),
    extras character varying(255),
    accountno character varying(28),
    bankname character varying(50),
    personal_no character(11),
    stats_no character varying(14),
    personal_id_no character varying(12),
    issuepost character varying(50),
    issuedate date,
    syncit smallint,
    cellphone_no character varying(16),
    email character varying(64),
    latitude numeric(15,12),
    longitude numeric(15,12),
    wetlicenceno character varying(12),
    henttype character(1) NOT NULL,
    season integer NOT NULL,
    mig_id integer,
    CONSTRAINT hents_henttype_check CHECK (((henttype = 'I'::bpchar) OR (henttype = 'C'::bpchar)))
);


ALTER TABLE public.hents OWNER TO postgres;

--
-- TOC entry 205 (class 1259 OID 17146)
-- Name: hents_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.hents_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.hents_seq OWNER TO postgres;

--
-- TOC entry 206 (class 1259 OID 17148)
-- Name: herds; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.herds (
    herd_id integer NOT NULL,
    herdno integer NOT NULL,
    herdname character varying(30) NOT NULL,
    zip character varying(10) NOT NULL,
    city character varying(48) NOT NULL,
    street character varying(48) NOT NULL,
    pobox character varying(10) NOT NULL,
    longitude numeric(15,12),
    latitude numeric(15,12),
    farm integer NOT NULL
);


ALTER TABLE public.herds OWNER TO postgres;

--
-- TOC entry 207 (class 1259 OID 17151)
-- Name: herds_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.herds_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.herds_seq OWNER TO postgres;

--
-- TOC entry 208 (class 1259 OID 17153)
-- Name: indoc_cows; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.indoc_cows (
    cow integer NOT NULL,
    doc integer NOT NULL,
    "group" smallint,
    stock integer,
    weight numeric(15,5),
    season integer NOT NULL,
    class integer
);


ALTER TABLE public.indoc_cows OWNER TO postgres;

--
-- TOC entry 209 (class 1259 OID 17156)
-- Name: indocs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.indocs (
    docdate date NOT NULL,
    extras character varying(255),
    plateno character varying(20),
    loaddate date NOT NULL,
    docid integer NOT NULL,
    agent integer,
    motive character(1) NOT NULL,
    season integer NOT NULL,
    herd integer NOT NULL,
    loadstrtm integer,
    loadendtm integer,
    mig_id integer,
    CONSTRAINT indocs_motive_check CHECK (((motive = 'B'::bpchar) OR (motive = '+'::bpchar) OR (motive = 'I'::bpchar)))
);


ALTER TABLE public.indocs OWNER TO postgres;

--
-- TOC entry 210 (class 1259 OID 17160)
-- Name: indocs_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.indocs_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.indocs_seq OWNER TO postgres;

--
-- TOC entry 211 (class 1259 OID 17162)
-- Name: movedoc_cows; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.movedoc_cows (
    cow integer NOT NULL,
    doc integer NOT NULL,
    "group" smallint,
    stock integer,
    class integer,
    weight numeric(15,5),
    season integer NOT NULL
);


ALTER TABLE public.movedoc_cows OWNER TO postgres;

--
-- TOC entry 212 (class 1259 OID 17165)
-- Name: movedocs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.movedocs (
    season integer NOT NULL,
    herd_from integer NOT NULL,
    herd_to integer NOT NULL,
    docdate date NOT NULL,
    extras character varying(255),
    plateno character varying(16),
    loaddate date NOT NULL,
    docid integer NOT NULL,
    loadstrtm integer,
    loadendtm integer,
    agent integer
);


ALTER TABLE public.movedocs OWNER TO postgres;

--
-- TOC entry 213 (class 1259 OID 17168)
-- Name: movedocs_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.movedocs_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.movedocs_seq OWNER TO postgres;

--
-- TOC entry 214 (class 1259 OID 17170)
-- Name: outdoc_cows; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.outdoc_cows (
    cow integer NOT NULL,
    doc integer NOT NULL,
    season integer NOT NULL,
    weight numeric(15,5),
    class integer,
    stock integer,
    "group" smallint
);


ALTER TABLE public.outdoc_cows OWNER TO postgres;

--
-- TOC entry 215 (class 1259 OID 17173)
-- Name: outdocs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.outdocs (
    docid integer NOT NULL,
    docdate date NOT NULL,
    extras character varying(255),
    plateno character varying(20),
    loaddate date NOT NULL,
    loadstrtm integer,
    loadendtm integer,
    agent integer,
    motive character(1) NOT NULL,
    season integer NOT NULL,
    herd integer NOT NULL,
    CONSTRAINT outdocs_motive_check CHECK (((motive = '-'::bpchar) OR (motive = 'E'::bpchar) OR (motive = 'D'::bpchar) OR (motive = 'K'::bpchar) OR (motive = 'S'::bpchar)))
);


ALTER TABLE public.outdocs OWNER TO postgres;

--
-- TOC entry 216 (class 1259 OID 17177)
-- Name: outdocs_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.outdocs_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.outdocs_seq OWNER TO postgres;

--
-- TOC entry 217 (class 1259 OID 17179)
-- Name: passrecgntnprofiles_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.passrecgntnprofiles_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.passrecgntnprofiles_seq OWNER TO postgres;

--
-- TOC entry 218 (class 1259 OID 17181)
-- Name: printprofiles_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.printprofiles_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.printprofiles_seq OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 17183)
-- Name: seasons; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.seasons (
    season_id integer NOT NULL,
    season_name character varying(50) NOT NULL,
    start_date date NOT NULL,
    end_date date NOT NULL,
    farm integer NOT NULL
);


ALTER TABLE public.seasons OWNER TO postgres;

--
-- TOC entry 239 (class 1259 OID 24579)
-- Name: seasons_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.seasons_seq
    START WITH 27
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.seasons_seq OWNER TO postgres;

--
-- TOC entry 220 (class 1259 OID 17186)
-- Name: sell_invoice_deductions; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.sell_invoice_deductions (
    season integer NOT NULL,
    invoice integer NOT NULL,
    code character varying(10) NOT NULL,
    fraction numeric(15,5) NOT NULL,
    reason character varying(300) NOT NULL,
    enabled integer NOT NULL,
    id integer DEFAULT nextval('public.invoicedeductions_seq'::regclass) NOT NULL
);


ALTER TABLE public.sell_invoice_deductions OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 17190)
-- Name: sell_invoice_entries; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.sell_invoice_entries (
    cow integer NOT NULL,
    invoice integer NOT NULL,
    price numeric(15,5) NOT NULL,
    class integer NOT NULL,
    weight numeric(15,5) NOT NULL,
    season integer NOT NULL
);


ALTER TABLE public.sell_invoice_entries OWNER TO postgres;

--
-- TOC entry 222 (class 1259 OID 17193)
-- Name: sell_invoice_hents; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.sell_invoice_hents (
    name character varying(150) NOT NULL,
    zip character varying(10),
    city character varying(48),
    street character varying(48),
    pobox character varying(10),
    fiscal_no character(10),
    accountno character(28),
    bankname character varying(50),
    personal_no character(11),
    personal_id_no character varying(12),
    issuepost character varying(50),
    issuedate date,
    season integer NOT NULL,
    parent_hent integer NOT NULL,
    invoice integer NOT NULL,
    hentno character(14) NOT NULL
);


ALTER TABLE public.sell_invoice_hents OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 17196)
-- Name: sell_invoices; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.sell_invoices (
    invoiceno integer NOT NULL,
    invoiceid integer NOT NULL,
    extras character varying(255),
    invoicedate date NOT NULL,
    paid date,
    invoice_type character(1) NOT NULL,
    vatrate numeric(15,5) NOT NULL,
    payway character(1) NOT NULL,
    payduedays integer NOT NULL,
    transactionplace character varying(48),
    transactiondate date,
    season integer NOT NULL,
    customnumber character varying(15),
    invoicescope character(1) NOT NULL,
    correct_for integer,
    mig_id integer,
    CONSTRAINT sell_invoices_invoice_type_check CHECK (((invoice_type = 'L'::bpchar) OR (invoice_type = 'V'::bpchar))),
    CONSTRAINT sell_invoices_invoicescope_check CHECK (((invoicescope = 'C'::bpchar) OR (invoicescope = 'D'::bpchar))),
    CONSTRAINT sell_invoices_payduedays_check CHECK (((payduedays >= 0) AND (payduedays <= 365))),
    CONSTRAINT sell_invoices_payway_check CHECK (((payway = '$'::bpchar) OR (payway = '@'::bpchar)))
);


ALTER TABLE public.sell_invoices OWNER TO postgres;

--
-- TOC entry 224 (class 1259 OID 17203)
-- Name: selldoc_cows; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.selldoc_cows (
    cow integer NOT NULL,
    doc integer NOT NULL,
    season integer NOT NULL,
    class integer,
    stock integer,
    "group" smallint,
    weight numeric(15,5)
);


ALTER TABLE public.selldoc_cows OWNER TO postgres;

--
-- TOC entry 225 (class 1259 OID 17206)
-- Name: selldocs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.selldocs (
    season integer NOT NULL,
    herd integer NOT NULL,
    hent integer NOT NULL,
    docdate date NOT NULL,
    extras character varying(255),
    plateno character varying(20),
    loaddate date NOT NULL,
    docid integer NOT NULL,
    invoice integer,
    loadstrtm integer,
    loadendtm integer,
    agent integer,
    motive character(1) NOT NULL,
    mig_id integer,
    CONSTRAINT selldocs_motive_check CHECK (((motive = '-'::bpchar) OR (motive = 'E'::bpchar) OR (motive = 'D'::bpchar) OR (motive = 'K'::bpchar) OR (motive = 'S'::bpchar)))
);


ALTER TABLE public.selldocs OWNER TO postgres;

--
-- TOC entry 226 (class 1259 OID 17210)
-- Name: selldocs_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.selldocs_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.selldocs_seq OWNER TO postgres;

--
-- TOC entry 227 (class 1259 OID 17212)
-- Name: sellinvoices_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.sellinvoices_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.sellinvoices_seq OWNER TO postgres;

--
-- TOC entry 228 (class 1259 OID 17214)
-- Name: stocks; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.stocks (
    stockname character varying(64),
    stockcode character varying(8) NOT NULL,
    stockid integer NOT NULL,
    farm integer NOT NULL
);


ALTER TABLE public.stocks OWNER TO postgres;

--
-- TOC entry 229 (class 1259 OID 17217)
-- Name: stocks_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.stocks_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.stocks_seq OWNER TO postgres;

--
-- TOC entry 230 (class 1259 OID 17219)
-- Name: user_deduction_definitions; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_deduction_definitions (
    deduction_id integer NOT NULL,
    deduction_code character varying(10) NOT NULL,
    fraction numeric(15,5) NOT NULL,
    reason character varying(300) NOT NULL,
    always integer NOT NULL,
    user_id integer NOT NULL,
    CONSTRAINT always_boolean CHECK (((always = 1) OR (always = 0)))
);


ALTER TABLE public.user_deduction_definitions OWNER TO postgres;

--
-- TOC entry 231 (class 1259 OID 17223)
-- Name: user_pass_recgntn_profile_fields; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_pass_recgntn_profile_fields (
    profile integer NOT NULL,
    field_name character varying(30) NOT NULL,
    x integer NOT NULL,
    y integer NOT NULL,
    w integer NOT NULL,
    h integer NOT NULL,
    user_owner integer NOT NULL
);


ALTER TABLE public.user_pass_recgntn_profile_fields OWNER TO postgres;

--
-- TOC entry 232 (class 1259 OID 17226)
-- Name: user_pass_recgntn_profiles; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_pass_recgntn_profiles (
    profile_id integer NOT NULL,
    profile_name character varying(40) NOT NULL,
    dimx integer NOT NULL,
    dimy integer NOT NULL,
    user_owner integer NOT NULL
);


ALTER TABLE public.user_pass_recgntn_profiles OWNER TO postgres;

--
-- TOC entry 233 (class 1259 OID 17229)
-- Name: user_print_profile_options; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_print_profile_options (
    option_code character varying(30) NOT NULL,
    profile integer NOT NULL,
    option_value character varying(1024),
    user_owner integer NOT NULL
);


ALTER TABLE public.user_print_profile_options OWNER TO postgres;

--
-- TOC entry 234 (class 1259 OID 17235)
-- Name: user_print_profile_order; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_print_profile_order (
    profile integer NOT NULL,
    user_owner integer NOT NULL,
    field_code character varying(30) NOT NULL,
    "collation" integer NOT NULL,
    "position" integer NOT NULL
);


ALTER TABLE public.user_print_profile_order OWNER TO postgres;

--
-- TOC entry 235 (class 1259 OID 17238)
-- Name: user_print_profiles; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_print_profiles (
    profile_id integer NOT NULL,
    profile_name character varying(60) NOT NULL,
    user_owner integer NOT NULL,
    document_code character varying(50) NOT NULL
);


ALTER TABLE public.user_print_profiles OWNER TO postgres;

--
-- TOC entry 236 (class 1259 OID 17241)
-- Name: user_settings; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_settings (
    userid integer NOT NULL,
    setting_name character varying(50) NOT NULL,
    setting_value character varying(255),
    part_order integer,
    assoc_key character varying(255)
);


ALTER TABLE public.user_settings OWNER TO postgres;

--
-- TOC entry 237 (class 1259 OID 17247)
-- Name: user_settings_definitions; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.user_settings_definitions (
    setting_name_pattern character varying(50) NOT NULL,
    collection integer NOT NULL,
    association integer NOT NULL,
    value_type character(1) NOT NULL,
    key_type character(1),
    pattern_order integer NOT NULL,
    CONSTRAINT user_settings_definitions_association_check CHECK (((association = 1) OR (association = 0))),
    CONSTRAINT user_settings_definitions_collection_check CHECK (((collection = 1) OR (collection = 0))),
    CONSTRAINT user_settings_definitions_key_type_check CHECK (((key_type = 'S'::bpchar) OR (key_type = 'I'::bpchar))),
    CONSTRAINT user_settings_definitions_value_type_check CHECK (((value_type = 'S'::bpchar) OR (value_type = 'I'::bpchar) OR (value_type = 'D'::bpchar) OR (value_type = 'U'::bpchar) OR (value_type = 'B'::bpchar)))
);


ALTER TABLE public.user_settings_definitions OWNER TO postgres;

--
-- TOC entry 238 (class 1259 OID 17254)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    username character varying(16) NOT NULL,
    userid integer NOT NULL,
    fname character varying(24) NOT NULL,
    lname character varying(32),
    pass character varying(16),
    lastlogin timestamp without time zone,
    privledges integer NOT NULL,
    farm integer NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 2209 (class 2606 OID 17270)
-- Name: buy_invoice_deductions buy_invoice_deductions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_deductions
    ADD CONSTRAINT buy_invoice_deductions_pkey PRIMARY KEY (id);


--
-- TOC entry 2211 (class 2606 OID 17272)
-- Name: buy_invoice_entries buy_invoice_entries_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_entries
    ADD CONSTRAINT buy_invoice_entries_pkey PRIMARY KEY (cow, invoice);


--
-- TOC entry 2213 (class 2606 OID 17274)
-- Name: buy_invoice_hents buy_invoice_hents_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_hents
    ADD CONSTRAINT buy_invoice_hents_pkey PRIMARY KEY (invoice);


--
-- TOC entry 2215 (class 2606 OID 17276)
-- Name: buy_invoices buy_invoices_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoices
    ADD CONSTRAINT buy_invoices_pkey PRIMARY KEY (invoiceid);


--
-- TOC entry 2217 (class 2606 OID 17278)
-- Name: buydoc_cows buydocs_cows_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydoc_cows
    ADD CONSTRAINT buydocs_cows_pkey PRIMARY KEY (cow, doc);


--
-- TOC entry 2219 (class 2606 OID 17280)
-- Name: buydocs buydocs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydocs
    ADD CONSTRAINT buydocs_pkey PRIMARY KEY (docid);


--
-- TOC entry 2221 (class 2606 OID 17282)
-- Name: cattle cattle_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_pkey PRIMARY KEY (animalid);


--
-- TOC entry 2225 (class 2606 OID 17284)
-- Name: counters counters_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.counters
    ADD CONSTRAINT counters_pkey PRIMARY KEY (name, season);


--
-- TOC entry 2227 (class 2606 OID 17286)
-- Name: farm_bank_accounts farm_bank_accounts_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.farm_bank_accounts
    ADD CONSTRAINT farm_bank_accounts_pkey PRIMARY KEY (account_id);


--
-- TOC entry 2229 (class 2606 OID 17288)
-- Name: farms farms_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.farms
    ADD CONSTRAINT farms_pk PRIMARY KEY (farm_id);


--
-- TOC entry 2231 (class 2606 OID 17290)
-- Name: hents hents_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.hents
    ADD CONSTRAINT hents_pkey PRIMARY KEY (hentid);


--
-- TOC entry 2233 (class 2606 OID 17292)
-- Name: herds herds_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.herds
    ADD CONSTRAINT herds_pkey PRIMARY KEY (herd_id);


--
-- TOC entry 2235 (class 2606 OID 17294)
-- Name: indoc_cows indoc_cows_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indoc_cows
    ADD CONSTRAINT indoc_cows_pkey PRIMARY KEY (cow, doc);


--
-- TOC entry 2237 (class 2606 OID 17296)
-- Name: indocs indocs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indocs
    ADD CONSTRAINT indocs_pkey PRIMARY KEY (docid);


--
-- TOC entry 2239 (class 2606 OID 17298)
-- Name: movedoc_cows movedoc_cows_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedoc_cows
    ADD CONSTRAINT movedoc_cows_pkey PRIMARY KEY (cow, doc);


--
-- TOC entry 2241 (class 2606 OID 17300)
-- Name: movedocs movedocs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedocs
    ADD CONSTRAINT movedocs_pkey PRIMARY KEY (docid);


--
-- TOC entry 2243 (class 2606 OID 17302)
-- Name: outdoc_cows outdoc_cows_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdoc_cows
    ADD CONSTRAINT outdoc_cows_pkey PRIMARY KEY (cow, doc);


--
-- TOC entry 2245 (class 2606 OID 17304)
-- Name: outdocs outdocs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdocs
    ADD CONSTRAINT outdocs_pkey PRIMARY KEY (docid);


--
-- TOC entry 2207 (class 2606 OID 17306)
-- Name: agents pk_agents; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.agents
    ADD CONSTRAINT pk_agents PRIMARY KEY (agentid);


--
-- TOC entry 2223 (class 2606 OID 17308)
-- Name: classes pk_classes; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.classes
    ADD CONSTRAINT pk_classes PRIMARY KEY (class_id);


--
-- TOC entry 2247 (class 2606 OID 17310)
-- Name: seasons pk_seasons; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.seasons
    ADD CONSTRAINT pk_seasons PRIMARY KEY (season_id);


--
-- TOC entry 2261 (class 2606 OID 17312)
-- Name: stocks pk_stocks; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stocks
    ADD CONSTRAINT pk_stocks PRIMARY KEY (stockid);


--
-- TOC entry 2277 (class 2606 OID 17314)
-- Name: users pk_users; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT pk_users PRIMARY KEY (userid);


--
-- TOC entry 2257 (class 2606 OID 17316)
-- Name: selldoc_cows seldoc_cows_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldoc_cows
    ADD CONSTRAINT seldoc_cows_pkey PRIMARY KEY (cow, doc);


--
-- TOC entry 2249 (class 2606 OID 17318)
-- Name: sell_invoice_deductions sell_invoice_deductions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_deductions
    ADD CONSTRAINT sell_invoice_deductions_pkey PRIMARY KEY (id);


--
-- TOC entry 2251 (class 2606 OID 17320)
-- Name: sell_invoice_entries sell_invoice_entries_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_entries
    ADD CONSTRAINT sell_invoice_entries_pkey PRIMARY KEY (cow, invoice);


--
-- TOC entry 2253 (class 2606 OID 17322)
-- Name: sell_invoice_hents sell_invoice_hents_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_hents
    ADD CONSTRAINT sell_invoice_hents_pkey PRIMARY KEY (invoice);


--
-- TOC entry 2255 (class 2606 OID 17324)
-- Name: sell_invoices sell_invoices_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoices
    ADD CONSTRAINT sell_invoices_pkey PRIMARY KEY (invoiceid);


--
-- TOC entry 2259 (class 2606 OID 17326)
-- Name: selldocs selldocs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldocs
    ADD CONSTRAINT selldocs_pkey PRIMARY KEY (docid);


--
-- TOC entry 2263 (class 2606 OID 17328)
-- Name: user_deduction_definitions user_deduction_definitions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_deduction_definitions
    ADD CONSTRAINT user_deduction_definitions_pkey PRIMARY KEY (deduction_id);


--
-- TOC entry 2265 (class 2606 OID 17330)
-- Name: user_pass_recgntn_profile_fields user_pass_recgntn_profile_fields_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_pass_recgntn_profile_fields
    ADD CONSTRAINT user_pass_recgntn_profile_fields_pkey PRIMARY KEY (profile, field_name);


--
-- TOC entry 2267 (class 2606 OID 17332)
-- Name: user_pass_recgntn_profiles user_pass_recgntn_profiles_pkey1; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_pass_recgntn_profiles
    ADD CONSTRAINT user_pass_recgntn_profiles_pkey1 PRIMARY KEY (profile_id);


--
-- TOC entry 2269 (class 2606 OID 17334)
-- Name: user_print_profile_options user_print_profile_options_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_print_profile_options
    ADD CONSTRAINT user_print_profile_options_pkey PRIMARY KEY (option_code, profile);


--
-- TOC entry 2271 (class 2606 OID 17336)
-- Name: user_print_profile_order user_print_profile_order_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_print_profile_order
    ADD CONSTRAINT user_print_profile_order_pkey PRIMARY KEY (profile, user_owner, field_code);


--
-- TOC entry 2273 (class 2606 OID 17338)
-- Name: user_print_profiles user_print_profiles_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_print_profiles
    ADD CONSTRAINT user_print_profiles_pkey PRIMARY KEY (profile_id);


--
-- TOC entry 2275 (class 2606 OID 17340)
-- Name: user_settings_definitions user_settings_definitions_setings_name_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_settings_definitions
    ADD CONSTRAINT user_settings_definitions_setings_name_key UNIQUE (setting_name_pattern);


--
-- TOC entry 2278 (class 2606 OID 17341)
-- Name: agents agents_farm_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.agents
    ADD CONSTRAINT agents_farm_fkey FOREIGN KEY (farm) REFERENCES public.farms(farm_id);


--
-- TOC entry 2279 (class 2606 OID 17346)
-- Name: buy_invoice_deductions buy_invoice_deductions_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_deductions
    ADD CONSTRAINT buy_invoice_deductions_invoice_fkey FOREIGN KEY (invoice) REFERENCES public.buy_invoices(invoiceid);


--
-- TOC entry 2280 (class 2606 OID 17351)
-- Name: buy_invoice_deductions buy_invoice_deductions_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_deductions
    ADD CONSTRAINT buy_invoice_deductions_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2281 (class 2606 OID 17356)
-- Name: buy_invoice_entries buy_invoice_entries_class_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_entries
    ADD CONSTRAINT buy_invoice_entries_class_fkey FOREIGN KEY (class) REFERENCES public.classes(class_id);


--
-- TOC entry 2282 (class 2606 OID 17361)
-- Name: buy_invoice_entries buy_invoice_entries_cow_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_entries
    ADD CONSTRAINT buy_invoice_entries_cow_fkey FOREIGN KEY (cow) REFERENCES public.cattle(animalid);


--
-- TOC entry 2283 (class 2606 OID 17366)
-- Name: buy_invoice_entries buy_invoice_entries_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_entries
    ADD CONSTRAINT buy_invoice_entries_invoice_fkey FOREIGN KEY (invoice) REFERENCES public.buy_invoices(invoiceid);


--
-- TOC entry 2284 (class 2606 OID 17371)
-- Name: buy_invoice_entries buy_invoice_entries_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_entries
    ADD CONSTRAINT buy_invoice_entries_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2285 (class 2606 OID 17376)
-- Name: buy_invoice_hents buy_invoice_hents_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_hents
    ADD CONSTRAINT buy_invoice_hents_invoice_fkey FOREIGN KEY (invoice) REFERENCES public.buy_invoices(invoiceid);


--
-- TOC entry 2286 (class 2606 OID 17381)
-- Name: buy_invoice_hents buy_invoice_hents_parent_hent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_hents
    ADD CONSTRAINT buy_invoice_hents_parent_hent_fkey FOREIGN KEY (parent_hent) REFERENCES public.hents(hentid);


--
-- TOC entry 2287 (class 2606 OID 17386)
-- Name: buy_invoice_hents buy_invoice_hents_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buy_invoice_hents
    ADD CONSTRAINT buy_invoice_hents_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2291 (class 2606 OID 17391)
-- Name: buydocs buydocs_agent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydocs
    ADD CONSTRAINT buydocs_agent_fkey FOREIGN KEY (agent) REFERENCES public.agents(agentid);


--
-- TOC entry 2288 (class 2606 OID 17396)
-- Name: buydoc_cows buydocs_cows_cow_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydoc_cows
    ADD CONSTRAINT buydocs_cows_cow_fkey FOREIGN KEY (cow) REFERENCES public.cattle(animalid);


--
-- TOC entry 2289 (class 2606 OID 17401)
-- Name: buydoc_cows buydocs_cows_doc_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydoc_cows
    ADD CONSTRAINT buydocs_cows_doc_fkey FOREIGN KEY (doc) REFERENCES public.buydocs(docid);


--
-- TOC entry 2290 (class 2606 OID 17406)
-- Name: buydoc_cows buydocs_cows_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydoc_cows
    ADD CONSTRAINT buydocs_cows_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2292 (class 2606 OID 17411)
-- Name: buydocs buydocs_hent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydocs
    ADD CONSTRAINT buydocs_hent_fkey FOREIGN KEY (hent) REFERENCES public.hents(hentid);


--
-- TOC entry 2293 (class 2606 OID 17416)
-- Name: buydocs buydocs_herd_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydocs
    ADD CONSTRAINT buydocs_herd_fkey FOREIGN KEY (herd) REFERENCES public.herds(herd_id);


--
-- TOC entry 2294 (class 2606 OID 17421)
-- Name: buydocs buydocs_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydocs
    ADD CONSTRAINT buydocs_invoice_fkey FOREIGN KEY (invoice) REFERENCES public.buy_invoices(invoiceid);


--
-- TOC entry 2295 (class 2606 OID 17426)
-- Name: buydocs buydocs_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.buydocs
    ADD CONSTRAINT buydocs_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2296 (class 2606 OID 17431)
-- Name: cattle cattle_buy_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_buy_invoice_fkey FOREIGN KEY (buy_invoice) REFERENCES public.buy_invoices(invoiceid);


--
-- TOC entry 2297 (class 2606 OID 17436)
-- Name: cattle cattle_class_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_class_fkey FOREIGN KEY (class) REFERENCES public.classes(class_id);


--
-- TOC entry 2298 (class 2606 OID 17441)
-- Name: cattle cattle_fstownr_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_fstownr_fkey FOREIGN KEY (fstownr) REFERENCES public.hents(hentid);


--
-- TOC entry 2299 (class 2606 OID 17446)
-- Name: cattle cattle_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2300 (class 2606 OID 17451)
-- Name: cattle cattle_sell_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_sell_invoice_fkey FOREIGN KEY (sell_invoice) REFERENCES public.sell_invoices(invoiceid);


--
-- TOC entry 2301 (class 2606 OID 17456)
-- Name: cattle cattle_stock_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_stock_fkey FOREIGN KEY (stock) REFERENCES public.stocks(stockid);


--
-- TOC entry 2302 (class 2606 OID 17461)
-- Name: cattle cattle_termbuyclass_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_termbuyclass_fkey FOREIGN KEY (termbuyclass) REFERENCES public.classes(class_id);


--
-- TOC entry 2303 (class 2606 OID 17466)
-- Name: cattle cattle_termsellclass_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cattle
    ADD CONSTRAINT cattle_termsellclass_fkey FOREIGN KEY (termsellclass) REFERENCES public.classes(class_id);


--
-- TOC entry 2305 (class 2606 OID 17471)
-- Name: counters counters_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.counters
    ADD CONSTRAINT counters_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2306 (class 2606 OID 17476)
-- Name: farm_bank_accounts farm_bank_accounts_farm_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.farm_bank_accounts
    ADD CONSTRAINT farm_bank_accounts_farm_fkey FOREIGN KEY (farm) REFERENCES public.farms(farm_id);


--
-- TOC entry 2304 (class 2606 OID 17481)
-- Name: classes fk_classes_farm; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.classes
    ADD CONSTRAINT fk_classes_farm FOREIGN KEY (farm) REFERENCES public.farms(farm_id);


--
-- TOC entry 2351 (class 2606 OID 17486)
-- Name: stocks fk_stocks_farms; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stocks
    ADD CONSTRAINT fk_stocks_farms FOREIGN KEY (farm) REFERENCES public.farms(farm_id);


--
-- TOC entry 2307 (class 2606 OID 17491)
-- Name: hents hents_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.hents
    ADD CONSTRAINT hents_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2308 (class 2606 OID 17496)
-- Name: herds herds_farm_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.herds
    ADD CONSTRAINT herds_farm_fkey FOREIGN KEY (farm) REFERENCES public.farms(farm_id);


--
-- TOC entry 2309 (class 2606 OID 17501)
-- Name: indoc_cows indoc_cows_class_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indoc_cows
    ADD CONSTRAINT indoc_cows_class_fkey FOREIGN KEY (class) REFERENCES public.classes(class_id);


--
-- TOC entry 2310 (class 2606 OID 17506)
-- Name: indoc_cows indoc_cows_cow_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indoc_cows
    ADD CONSTRAINT indoc_cows_cow_fkey FOREIGN KEY (cow) REFERENCES public.cattle(animalid);


--
-- TOC entry 2311 (class 2606 OID 17511)
-- Name: indoc_cows indoc_cows_doc_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indoc_cows
    ADD CONSTRAINT indoc_cows_doc_fkey FOREIGN KEY (doc) REFERENCES public.indocs(docid);


--
-- TOC entry 2312 (class 2606 OID 17516)
-- Name: indoc_cows indoc_cows_stock_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indoc_cows
    ADD CONSTRAINT indoc_cows_stock_fkey FOREIGN KEY (stock) REFERENCES public.stocks(stockid);


--
-- TOC entry 2313 (class 2606 OID 17521)
-- Name: indocs indocs_agent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indocs
    ADD CONSTRAINT indocs_agent_fkey FOREIGN KEY (agent) REFERENCES public.agents(agentid);


--
-- TOC entry 2314 (class 2606 OID 17526)
-- Name: indocs indocs_herd_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indocs
    ADD CONSTRAINT indocs_herd_fkey FOREIGN KEY (herd) REFERENCES public.herds(herd_id);


--
-- TOC entry 2315 (class 2606 OID 17531)
-- Name: indocs indocs_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.indocs
    ADD CONSTRAINT indocs_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2316 (class 2606 OID 17536)
-- Name: movedoc_cows movedoc_cows_cow_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedoc_cows
    ADD CONSTRAINT movedoc_cows_cow_fkey FOREIGN KEY (cow) REFERENCES public.cattle(animalid);


--
-- TOC entry 2317 (class 2606 OID 17541)
-- Name: movedoc_cows movedoc_cows_doc_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedoc_cows
    ADD CONSTRAINT movedoc_cows_doc_fkey FOREIGN KEY (doc) REFERENCES public.movedocs(docid);


--
-- TOC entry 2318 (class 2606 OID 17546)
-- Name: movedoc_cows movedoc_cows_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedoc_cows
    ADD CONSTRAINT movedoc_cows_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2319 (class 2606 OID 17551)
-- Name: movedocs movedocs_agent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedocs
    ADD CONSTRAINT movedocs_agent_fkey FOREIGN KEY (agent) REFERENCES public.agents(agentid);


--
-- TOC entry 2320 (class 2606 OID 17556)
-- Name: movedocs movedocs_herd_from_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedocs
    ADD CONSTRAINT movedocs_herd_from_fkey FOREIGN KEY (herd_from) REFERENCES public.herds(herd_id);


--
-- TOC entry 2321 (class 2606 OID 17561)
-- Name: movedocs movedocs_herd_to_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedocs
    ADD CONSTRAINT movedocs_herd_to_fkey FOREIGN KEY (herd_to) REFERENCES public.herds(herd_id);


--
-- TOC entry 2322 (class 2606 OID 17566)
-- Name: movedocs movedocs_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.movedocs
    ADD CONSTRAINT movedocs_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2323 (class 2606 OID 17571)
-- Name: outdoc_cows outdoc_cows_class_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdoc_cows
    ADD CONSTRAINT outdoc_cows_class_fkey FOREIGN KEY (class) REFERENCES public.classes(class_id);


--
-- TOC entry 2324 (class 2606 OID 17576)
-- Name: outdoc_cows outdoc_cows_cow_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdoc_cows
    ADD CONSTRAINT outdoc_cows_cow_fkey FOREIGN KEY (cow) REFERENCES public.cattle(animalid);


--
-- TOC entry 2325 (class 2606 OID 17581)
-- Name: outdoc_cows outdoc_cows_doc_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdoc_cows
    ADD CONSTRAINT outdoc_cows_doc_fkey FOREIGN KEY (doc) REFERENCES public.outdocs(docid);


--
-- TOC entry 2326 (class 2606 OID 17586)
-- Name: outdoc_cows outdoc_cows_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdoc_cows
    ADD CONSTRAINT outdoc_cows_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2327 (class 2606 OID 17591)
-- Name: outdoc_cows outdoc_cows_stock_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdoc_cows
    ADD CONSTRAINT outdoc_cows_stock_fkey FOREIGN KEY (stock) REFERENCES public.stocks(stockid);


--
-- TOC entry 2328 (class 2606 OID 17596)
-- Name: outdocs outdocs_agent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdocs
    ADD CONSTRAINT outdocs_agent_fkey FOREIGN KEY (agent) REFERENCES public.agents(agentid);


--
-- TOC entry 2329 (class 2606 OID 17601)
-- Name: outdocs outdocs_herd_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdocs
    ADD CONSTRAINT outdocs_herd_fkey FOREIGN KEY (herd) REFERENCES public.herds(herd_id);


--
-- TOC entry 2330 (class 2606 OID 17606)
-- Name: outdocs outdocs_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outdocs
    ADD CONSTRAINT outdocs_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2331 (class 2606 OID 17611)
-- Name: seasons seasons_farm_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.seasons
    ADD CONSTRAINT seasons_farm_fkey FOREIGN KEY (farm) REFERENCES public.farms(farm_id);


--
-- TOC entry 2341 (class 2606 OID 17616)
-- Name: selldoc_cows seldoc_cows_class_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldoc_cows
    ADD CONSTRAINT seldoc_cows_class_fkey FOREIGN KEY (class) REFERENCES public.classes(class_id);


--
-- TOC entry 2342 (class 2606 OID 17621)
-- Name: selldoc_cows seldoc_cows_cow_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldoc_cows
    ADD CONSTRAINT seldoc_cows_cow_fkey FOREIGN KEY (cow) REFERENCES public.cattle(animalid);


--
-- TOC entry 2343 (class 2606 OID 17626)
-- Name: selldoc_cows seldoc_cows_doc_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldoc_cows
    ADD CONSTRAINT seldoc_cows_doc_fkey FOREIGN KEY (doc) REFERENCES public.selldocs(docid);


--
-- TOC entry 2344 (class 2606 OID 17631)
-- Name: selldoc_cows seldoc_cows_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldoc_cows
    ADD CONSTRAINT seldoc_cows_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2345 (class 2606 OID 17636)
-- Name: selldoc_cows seldoc_cows_stock_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldoc_cows
    ADD CONSTRAINT seldoc_cows_stock_fkey FOREIGN KEY (stock) REFERENCES public.stocks(stockid);


--
-- TOC entry 2332 (class 2606 OID 17641)
-- Name: sell_invoice_deductions sell_invoice_deductions_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_deductions
    ADD CONSTRAINT sell_invoice_deductions_invoice_fkey FOREIGN KEY (invoice) REFERENCES public.sell_invoices(invoiceid);


--
-- TOC entry 2333 (class 2606 OID 17646)
-- Name: sell_invoice_deductions sell_invoice_deductions_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_deductions
    ADD CONSTRAINT sell_invoice_deductions_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2334 (class 2606 OID 17651)
-- Name: sell_invoice_entries sell_invoice_entries_class_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_entries
    ADD CONSTRAINT sell_invoice_entries_class_fkey FOREIGN KEY (class) REFERENCES public.classes(class_id);


--
-- TOC entry 2335 (class 2606 OID 17656)
-- Name: sell_invoice_entries sell_invoice_entries_cow_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_entries
    ADD CONSTRAINT sell_invoice_entries_cow_fkey FOREIGN KEY (cow) REFERENCES public.cattle(animalid);


--
-- TOC entry 2336 (class 2606 OID 17661)
-- Name: sell_invoice_entries sell_invoice_entries_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_entries
    ADD CONSTRAINT sell_invoice_entries_invoice_fkey FOREIGN KEY (invoice) REFERENCES public.sell_invoices(invoiceid);


--
-- TOC entry 2337 (class 2606 OID 17666)
-- Name: sell_invoice_entries sell_invoice_entries_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_entries
    ADD CONSTRAINT sell_invoice_entries_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2338 (class 2606 OID 17671)
-- Name: sell_invoice_hents sell_invoice_hents_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_hents
    ADD CONSTRAINT sell_invoice_hents_invoice_fkey FOREIGN KEY (invoice) REFERENCES public.sell_invoices(invoiceid);


--
-- TOC entry 2339 (class 2606 OID 17676)
-- Name: sell_invoice_hents sell_invoice_hents_parent_hent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_hents
    ADD CONSTRAINT sell_invoice_hents_parent_hent_fkey FOREIGN KEY (parent_hent) REFERENCES public.hents(hentid);


--
-- TOC entry 2340 (class 2606 OID 17681)
-- Name: sell_invoice_hents sell_invoice_hents_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.sell_invoice_hents
    ADD CONSTRAINT sell_invoice_hents_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2346 (class 2606 OID 17686)
-- Name: selldocs selldocs_agent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldocs
    ADD CONSTRAINT selldocs_agent_fkey FOREIGN KEY (agent) REFERENCES public.agents(agentid);


--
-- TOC entry 2347 (class 2606 OID 17691)
-- Name: selldocs selldocs_hent_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldocs
    ADD CONSTRAINT selldocs_hent_fkey FOREIGN KEY (hent) REFERENCES public.hents(hentid);


--
-- TOC entry 2348 (class 2606 OID 17696)
-- Name: selldocs selldocs_herd_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldocs
    ADD CONSTRAINT selldocs_herd_fkey FOREIGN KEY (herd) REFERENCES public.herds(herd_id);


--
-- TOC entry 2349 (class 2606 OID 17701)
-- Name: selldocs selldocs_invoice_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldocs
    ADD CONSTRAINT selldocs_invoice_fkey FOREIGN KEY (invoice) REFERENCES public.sell_invoices(invoiceid);


--
-- TOC entry 2350 (class 2606 OID 17706)
-- Name: selldocs selldocs_season_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.selldocs
    ADD CONSTRAINT selldocs_season_fkey FOREIGN KEY (season) REFERENCES public.seasons(season_id);


--
-- TOC entry 2352 (class 2606 OID 17711)
-- Name: user_deduction_definitions user_deduction_definitions_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_deduction_definitions
    ADD CONSTRAINT user_deduction_definitions_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.users(userid);


--
-- TOC entry 2353 (class 2606 OID 17716)
-- Name: user_pass_recgntn_profile_fields user_pass_recgntn_profile_fields_profile_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_pass_recgntn_profile_fields
    ADD CONSTRAINT user_pass_recgntn_profile_fields_profile_fkey FOREIGN KEY (profile) REFERENCES public.user_pass_recgntn_profiles(profile_id);


--
-- TOC entry 2354 (class 2606 OID 17721)
-- Name: user_pass_recgntn_profile_fields user_pass_recgntn_profile_fields_user_owner_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_pass_recgntn_profile_fields
    ADD CONSTRAINT user_pass_recgntn_profile_fields_user_owner_fkey FOREIGN KEY (user_owner) REFERENCES public.users(userid);


--
-- TOC entry 2355 (class 2606 OID 17726)
-- Name: user_pass_recgntn_profiles user_pass_recgntn_profiles_user_owner_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_pass_recgntn_profiles
    ADD CONSTRAINT user_pass_recgntn_profiles_user_owner_fkey FOREIGN KEY (user_owner) REFERENCES public.users(userid);


--
-- TOC entry 2356 (class 2606 OID 17731)
-- Name: user_print_profile_options user_print_profile_options_profile_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_print_profile_options
    ADD CONSTRAINT user_print_profile_options_profile_fkey FOREIGN KEY (profile) REFERENCES public.user_print_profiles(profile_id);


--
-- TOC entry 2357 (class 2606 OID 17736)
-- Name: user_print_profile_options user_print_profile_options_user_owner_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_print_profile_options
    ADD CONSTRAINT user_print_profile_options_user_owner_fkey FOREIGN KEY (user_owner) REFERENCES public.users(userid);


--
-- TOC entry 2358 (class 2606 OID 17741)
-- Name: user_print_profile_order user_print_profile_order_profile_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_print_profile_order
    ADD CONSTRAINT user_print_profile_order_profile_fkey FOREIGN KEY (profile) REFERENCES public.user_print_profiles(profile_id);


--
-- TOC entry 2359 (class 2606 OID 17746)
-- Name: user_print_profile_order user_print_profile_order_user_owner_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_print_profile_order
    ADD CONSTRAINT user_print_profile_order_user_owner_fkey FOREIGN KEY (user_owner) REFERENCES public.users(userid);


--
-- TOC entry 2360 (class 2606 OID 17751)
-- Name: user_print_profiles user_print_profiles_user_owner_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_print_profiles
    ADD CONSTRAINT user_print_profiles_user_owner_fkey FOREIGN KEY (user_owner) REFERENCES public.users(userid);


--
-- TOC entry 2361 (class 2606 OID 17756)
-- Name: user_settings user_settings_userid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.user_settings
    ADD CONSTRAINT user_settings_userid_fkey FOREIGN KEY (userid) REFERENCES public.users(userid);


--
-- TOC entry 2362 (class 2606 OID 17761)
-- Name: users users_farm_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_farm_fkey FOREIGN KEY (farm) REFERENCES public.farms(farm_id);


-- Completed on 2023-05-09 21:53:06

--
-- PostgreSQL database dump complete
--


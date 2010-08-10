require File.expand_path('../../spec_helper', __FILE__)

describe DateTime do

  it "._strptime should be a hash of values" do
    DateTime._strptime('2008-10-11T01:00:02+0000').should == {:offset=>0, :zone=>"+0000", :sec=>2, :year=>2008, :hour=>1, :mday=>11, :min=>0, :mon=>10}
    DateTime._strptime('2008-10-11', '%Y-%m-%d').should == {:year=>2008, :mon=>10, :mday=>11}
  end

  it ".civil should have defaults and an optional sg value" do
    DateTime.civil(2008, 1, 1, 1, 1, 1, 0.5, 1).should == DateTime.civil(2008, 1, 1, 1, 1, 1, 0.5)
    DateTime.civil.should == DateTime.jd
  end

  it ".commercial should have defaults and an optional sg value" do
    DateTime.commercial(2008, 1, 1, 1, 1, 1, 0.5, 1).should == DateTime.commercial(2008, 1, 1, 1, 1, 1, 0.5)
    DateTime.commercial.should == (RUBY_VERSION > '1.9.0' ? DateTime.jd : DateTime.commercial(1582, 41, 5))
  end

  it ".jd should have defaults and an optional sg value" do
    DateTime.jd(2008, 1, 1, 1, 0.5, 1).should == DateTime.jd(2008, 1, 1, 1, 0.5)
    DateTime.jd.should == DateTime.jd(0, 0, 0, 0, 0)
  end

  it ".ordinal should have defaults and an optional sg value" do
    DateTime.ordinal(2008, 1, 1, 1, 1, 0.5, 1).should == DateTime.ordinal(2008, 1, 1, 1, 1, 0.5)
    DateTime.ordinal.should == DateTime.jd
  end

  it ".parse should have defaults and an optional sg value" do
    DateTime.parse('2008-10-11').should == DateTime.civil(2008, 10, 11)
    DateTime.parse('2008-10-11', true).should == DateTime.civil(2008, 10, 11)
    DateTime.parse('2008-10-11', true, 1).should == DateTime.civil(2008, 10, 11)
    DateTime.parse.should == DateTime.jd
  end

  it ".strptime should have defaults and an optional sg value" do
    DateTime.strptime('2008-10-11T00:00:00+00:00').should == DateTime.civil(2008, 10, 11)
    DateTime.strptime('2008-10-11', '%Y-%m-%d').should == DateTime.civil(2008, 10, 11)
    DateTime.strptime('2008-10-11', '%Y-%m-%d', 1).should == DateTime.civil(2008, 10, 11)
    DateTime.strptime.should == DateTime.jd
  end

  it ".now should have an optional sg value" do
    DateTime.now(1).to_s.should == DateTime.now.to_s
  end

  it "#asctime and #ctime should be a string with the date formatted" do
    DateTime.new(2008, 1, 1).asctime.should == 'Tue Jan  1 00:00:00 2008'
    DateTime.new(2008, 1, 1).ctime.should == 'Tue Jan  1 00:00:00 2008'
  end

  it "#day_fraction should be the fraction of the day" do
    DateTime.new(2008, 1, 1).day_fraction.should == 0.0
    DateTime.new(2008, 1, 1, 12).day_fraction.should == 0.5
  end

  it "#new_offset should be a separate datetime with a modified offset" do
    DateTime.new(2008, 1, 1).new_offset(0.5).should == DateTime.new(2008, 1, 1, 12, 0, 0, 0.5)
  end

  it "#offset should be the offset as a float fraction of the day" do
    DateTime.parse('2008-01-01 00:00:00+12:00').offset.should == 0.5
  end

  ruby_version_is "" ... "1.9" do
    it "#newof should be a separate datetime with a modified offset" do
      DateTime.new(2008, 1, 1).newof(0.5).should == DateTime.new(2008, 1, 1, 12, 0, 0, 0.5)
    end

    it "#of should be the offset as a float fraction of the day" do
      DateTime.parse('2008-01-01 00:00:00+12:00').of.should == 0.5
    end
  end

  it "#sec_fraction should be the fraction of a second as a fraction of the day" do
    DateTime.new(2008, 1, 1).sec_fraction.should == 0.0
    DateTime.parse('12:13:15.678900').sec_fraction.should be_close(7.85763888888889e-06, 0.0000000000001)
  end

end
